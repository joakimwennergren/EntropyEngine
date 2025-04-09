#include "vulkan_renderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ecs/components/2d_quad.h"
#include "ecs/components/dimension.h"
#include "ecs/components/position.h"
#include "ecs/components/texture.h"
#include "vulkan/data/data.h"

#include <iostream>

using namespace Entropy::Vulkan::Renderers;
using namespace Entropy::Graphics::Vulkan::Memory;
using namespace Entropy::Graphics::Vulkan::Devices;
using namespace Entropy::Graphics::Vulkan::SwapChains;
using namespace Entropy::Graphics::Vulkan::Synchronization;
using namespace Entropy::Graphics::Vulkan::Pipelines;
using namespace Entropy::Graphics::Vulkan::RenderPasses;
using namespace Entropy::Graphics::Vulkan::Buffers;
using namespace Entropy::Graphics::Vulkan::Data;
using namespace Entropy::Cameras;
using namespace Entropy::Assets;

VulkanRenderer::VulkanRenderer(const uint32_t width, const uint32_t height) {
  const ServiceLocator* sl = ServiceLocator::GetInstance();
  world_ = sl->getService<ECS::IWorld>();
  allocator_ = sl->getService<IAllocator>();
  logicalDevice_ = sl->getService<ILogicalDevice>();
  swapChain_ = sl->getService<ISwapChain>();
  cameraManager_ = sl->getService<ICameraManager>();
  assetManager_ = sl->getService<IAssetManager>();

  renderPass_ = std::make_shared<RenderPass>();
  renderPass_->RecreateDepthBuffer(width, height);
  renderPass_->CreateFrameBuffers(width, height);

  synchronizer_ = std::make_unique<Synchronizer>(CONCURRENT_FRAMES_IN_FLIGHT);
  two_d_pipeline_ = std::make_unique<TwoDPipeline>(renderPass_);

  for (uint32_t i = 0; i < CONCURRENT_FRAMES_IN_FLIGHT; i++) {
    commandBuffers_.emplace_back(
        std::make_shared<CommandBuffer>(VK_COMMAND_BUFFER_LEVEL_PRIMARY));
  }

  UBO_ = std::make_unique<UniformBuffer>(sizeof(UBOData));

  vertexDataBuffer_ = std::make_unique<VertexBuffer<TwoDVertex>>(
      MAX_INSTANCE_COUNT * sizeof(TwoDVertex));

  instanceDataBuffer_ = std::make_unique<VertexBuffer<InstanceData>>(
      MAX_INSTANCE_COUNT * sizeof(InstanceData));

  indexDataBuffer_ =
      std::make_unique<IndexBuffer<uint32_t>>(MAX_INSTANCE_COUNT);

  std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

  VkDescriptorBufferInfo uboInfo{};
  uboInfo.buffer = UBO_->GetVulkanBuffer();
  uboInfo.offset = 0;
  uboInfo.range = sizeof(UBOData);

  VkDescriptorImageInfo imageInfo;
  imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  imageInfo.imageView =
      assetManager_->textureAtlas.texture_->GetImageView()->Get();
  imageInfo.sampler = assetManager_->textureAtlas.texture_->GetSampler();

  descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  descriptorWrites[0].dstSet = two_d_pipeline_->descriptorSet;
  descriptorWrites[0].dstBinding = 1;
  descriptorWrites[0].dstArrayElement = 0;
  descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  descriptorWrites[0].descriptorCount = 1;
  descriptorWrites[0].pBufferInfo = &uboInfo;

  descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  descriptorWrites[1].dstSet = two_d_pipeline_->descriptorSet;
  descriptorWrites[1].dstBinding = 2;
  descriptorWrites[1].dstArrayElement = 0;
  descriptorWrites[1].descriptorType =
      VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  descriptorWrites[1].descriptorCount = 1;
  descriptorWrites[1].pImageInfo = &imageInfo;

  vkUpdateDescriptorSets(logicalDevice_->Get(), descriptorWrites.size(),
                         descriptorWrites.data(), 0, nullptr);
}

void VulkanRenderer::Resize(const uint32_t width, const uint32_t height) {
  synchronizer_ = std::make_unique<Synchronizer>(CONCURRENT_FRAMES_IN_FLIGHT);
  swapChain_->RecreateSwapChain(width, height);
  renderPass_->RecreateDepthBuffer(width, height);
  renderPass_->RecreateFrameBuffers(width, height);
}

void VulkanRenderer::Render(const uint32_t width, const uint32_t height) {
  const auto currentFence = synchronizer_->GetFences()[currentFrame_];

  VK_CHECK(vkWaitForFences(logicalDevice_->Get(), 1, &currentFence, VK_TRUE,
                           UINT64_MAX));

  VK_CHECK(vkResetFences(logicalDevice_->Get(), 1, &currentFence));

  const auto imageResult = vkAcquireNextImageKHR(
      logicalDevice_->Get(), swapChain_->Get(), UINT64_MAX,
      synchronizer_->GetImageSemaphores()[currentFrame_], VK_NULL_HANDLE,
      &imageIndex_);

  if (imageResult == VK_SUBOPTIMAL_KHR ||
      imageResult == VK_ERROR_OUT_OF_DATE_KHR)
    return;

  const auto camera = cameraManager_->GetCurrentCamera();
  camera->SetPerspective(width, height, 0.1f, 1024.0f);

  commandBuffers_[currentFrame_]->Record();
  renderPass_->Begin(commandBuffers_[currentFrame_], imageIndex_);

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = {width, height};
  vkCmdSetScissor(commandBuffers_[currentFrame_]->Get(), 0, 1, &scissor);

  // Set Viewport
  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(width);
  viewport.height = static_cast<float>(height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(commandBuffers_[currentFrame_]->Get(), 0, 1, &viewport);

  vkCmdBindPipeline(commandBuffers_[currentFrame_]->Get(),
                    VK_PIPELINE_BIND_POINT_GRAPHICS,
                    two_d_pipeline_->GetPipeline());

  const auto ds0 = two_d_pipeline_->descriptorSet;
  vkCmdBindDescriptorSets(
      commandBuffers_[currentFrame_]->Get(), VK_PIPELINE_BIND_POINT_GRAPHICS,
      two_d_pipeline_->GetPipelineLayout(), 0, 1, &ds0, 0, nullptr);

  // Copy UBO data
  const UBOData ubo{camera->projection, camera->view};
  memcpy(UBO_->GetMappedMemory(), &ubo, sizeof(ubo));

  objectIndex_ = 0;
  indices.clear();
  vertices.clear();
  instanceData_.clear();

  world_->Get()->each<ECS::Components::TwoDQuad>(
      [&](const flecs::entity e, const ECS::Components::TwoDQuad& quad) {
        const auto& quadVertices = quad.vertices;
        const auto& quadIndices = quad.indices;

        vertices.insert(vertices.end(), quadVertices.begin(),
                        quadVertices.end());
        indices.insert(indices.end(), quadIndices.begin(), quadIndices.end());

        const auto position = e.has<ECS::Components::Position>()
                                  ? e.get_ref<ECS::Components::Position>()->pos
                                  : glm::vec3(0.0f);

        const auto scale = e.has<ECS::Components::Dimension>()
                               ? e.get_ref<ECS::Components::Dimension>()->scale
                               : glm::vec3(0.0f);

        instanceData_.push_back(
            InstanceData{position, glm::vec3{0.0f, 0.0f, 0.0f}, scale.x, 0});
        objectIndex_++;
      });

  vertexDataBuffer_->Update(vertices);
  indexDataBuffer_->Update(indices);
  instanceDataBuffer_->Update(instanceData_);

  const auto vertexBuffer = vertexDataBuffer_->GetVulkanBuffer();
  const auto instanceBuffer = instanceDataBuffer_->GetVulkanBuffer();

  constexpr VkDeviceSize offsets[1] = {0};
  vkCmdBindVertexBuffers(commandBuffers_[currentFrame_]->Get(), 0, 1,
                         &vertexBuffer, offsets);
  vkCmdBindVertexBuffers(commandBuffers_[currentFrame_]->Get(), 1, 1,
                         &instanceBuffer, offsets);
  vkCmdBindIndexBuffer(commandBuffers_[currentFrame_]->Get(),
                       indexDataBuffer_->GetVulkanBuffer(), 0,
                       VK_INDEX_TYPE_UINT32);

  // Draw current renderable
  vkCmdDrawIndexed(commandBuffers_[currentFrame_]->Get(), indices.size(),
                   objectIndex_, 0, 0, 0);

  // End render pass and command buffer recording
  RenderPass::End(commandBuffers_[currentFrame_]);
  commandBuffers_[currentFrame_]->EndRecording();

  constexpr VkPipelineStageFlags waitStages[] = {
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  const auto& waitSemaphores = synchronizer_->GetImageSemaphores();
  const auto& signalSemaphores = synchronizer_->GetRenderFinishedSemaphores();
  const auto& cmdBuffer = commandBuffers_[currentFrame_]->Get();
  const auto& swapChain = swapChain_->Get();

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = &waitSemaphores[currentFrame_];
  submitInfo.pWaitDstStageMask = waitStages;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &cmdBuffer;
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = &signalSemaphores[currentFrame_];

  // Submit queue
  VK_CHECK(vkQueueSubmit(logicalDevice_->GetGraphicQueue(), 1, &submitInfo,
                         synchronizer_->GetFences()[currentFrame_]));

  VkPresentInfoKHR presentInfo{};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.pNext = nullptr;
  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = &signalSemaphores[currentFrame_];
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = &swapChain;
  presentInfo.pImageIndices = &imageIndex_;
  vkQueuePresentKHR(logicalDevice_->GetGraphicQueue(), &presentInfo);

  currentFrame_ = (currentFrame_ + 1) % CONCURRENT_FRAMES_IN_FLIGHT;
}