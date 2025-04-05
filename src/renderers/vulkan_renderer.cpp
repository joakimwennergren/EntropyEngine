#include "vulkan_renderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "ecs/components/2d_quad.h"
#include "ecs/components/dimension.h"
#include "ecs/components/position.h"
#include "ecs/components/texture.h"
#include "vulkan/data/data.h"

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
  instanceData_ = std::make_unique<StorageBuffer>(MAX_INSTANCE_COUNT *
                                                  sizeof(InstanceData));
  blankTexture_ = std::make_unique<Graphics::Vulkan::Textures::Texture>(1, 1);

  std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

  VkDescriptorBufferInfo uboInfo{};
  uboInfo.buffer = UBO_->GetVulkanBuffer();
  uboInfo.offset = 0;
  uboInfo.range = sizeof(UBOData);

  VkDescriptorBufferInfo instanceInfo;
  instanceInfo.buffer = instanceData_->GetVulkanBuffer();
  instanceInfo.offset = 0;
  instanceInfo.range = sizeof(InstanceData) * MAX_INSTANCE_COUNT;

  descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  descriptorWrites[0].dstSet = two_d_pipeline_->descriptorSet;
  descriptorWrites[0].dstBinding = 1;
  descriptorWrites[0].dstArrayElement = 0;
  descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  descriptorWrites[0].descriptorCount = 1;
  descriptorWrites[0].pBufferInfo = &uboInfo;

  descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  descriptorWrites[1].dstSet = two_d_pipeline_->descriptorSet;
  descriptorWrites[1].dstBinding = 0;
  descriptorWrites[1].dstArrayElement = 0;
  descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
  descriptorWrites[1].descriptorCount = 1;
  descriptorWrites[1].pBufferInfo = &instanceInfo;

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
  const UBOData ubo{camera->perspective, camera->view};
  memcpy(UBO_->GetMappedMemory(), &ubo, sizeof(ubo));

  objectIndex_ = 0;
  indices.clear();
  vertices.clear();

  world_->Get()->each<ECS::Components::TwoDQuad>(
      [&](const flecs::entity e, const ECS::Components::TwoDQuad& quad) {
        vertices.insert(vertices.end(), quad.vertices.begin(),
                        quad.vertices.end());
        indices.insert(indices.end(), quad.indices.begin(), quad.indices.end());

        if (e.has<ECS::Components::Texture>()) {
          const auto texture =
              assetManager_->GetAsync<Graphics::Vulkan::Textures::Texture>(
                  e.get_ref<ECS::Components::Texture>()->index);

          if (texture) {
            imageInfos[objectIndex_].imageLayout =
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfos[objectIndex_].imageView = texture->GetImageView()->Get();
            imageInfos[objectIndex_].sampler = texture->GetSampler();
          } else {
            imageInfos[objectIndex_].imageLayout =
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfos[objectIndex_].imageView =
                blankTexture_->GetImageView()->Get();
            imageInfos[objectIndex_].sampler = blankTexture_->GetSampler();
          }
        } else {
          imageInfos[objectIndex_].imageLayout =
              VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
          imageInfos[objectIndex_].imageView =
              blankTexture_->GetImageView()->Get();
          imageInfos[objectIndex_].sampler = blankTexture_->GetSampler();
        }

        UpdateInstance(e);
        objectIndex_++;

      });

  for (uint32_t i = objectIndex_; i < TEXTURE_ARRAY_SIZE; i++) {
    imageInfos[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfos[i].imageView = blankTexture_->GetImageView()->Get();
    imageInfos[i].sampler = blankTexture_->GetSampler();
  }

  VkWriteDescriptorSet descriptorWrite{};
  descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  descriptorWrite.dstSet = two_d_pipeline_->descriptorSet;
  descriptorWrite.dstBinding = 2;
  descriptorWrite.dstArrayElement = 0;
  descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  descriptorWrite.descriptorCount = TEXTURE_ARRAY_SIZE;
  descriptorWrite.pImageInfo = imageInfos;  // Pass the array

  vkUpdateDescriptorSets(logicalDevice_->Get(), 1, &descriptorWrite, 0,
                         nullptr);

  if (indices.empty() || vertices.empty()) {
    return;
  }

  PushConstant constants{};
  constants.instanceIndex = objectIndex_;

  // upload the matrix to the GPU via push constants
  vkCmdPushConstants(commandBuffers_[currentFrame_]->Get(),
                     two_d_pipeline_->GetPipelineLayout(), VK_SHADER_STAGE_ALL,
                     0, sizeof(PushConstant), &constants);

  vertexDataBuffer_ = std::make_shared<VertexBuffer<TwoDVertex>>(vertices);
  indexDataBuffer_ = std::make_shared<IndexBuffer<uint32_t>>(indices);

  // Bind vertex & index buffers
  const VkBuffer vertexBuffers[] = {vertexDataBuffer_->GetVulkanBuffer()};
  constexpr VkDeviceSize offsets[] = {0};

  vkCmdBindVertexBuffers(commandBuffers_[currentFrame_]->Get(), 0, 1,
                         vertexBuffers, offsets);

  vkCmdBindIndexBuffer(commandBuffers_[currentFrame_]->Get(),
                       indexDataBuffer_->GetVulkanBuffer(), 0,
                       VK_INDEX_TYPE_UINT32);

  // Draw current renderable
  vkCmdDrawIndexed(commandBuffers_[currentFrame_]->Get(), indices.size(),
                   objectIndex_, 0, 0, 0);

  // End render pass and command buffer recording
  RenderPass::End(commandBuffers_[currentFrame_]);
  commandBuffers_[currentFrame_]->EndRecording();

  const auto cmdBuffer = commandBuffers_[currentFrame_]->Get();

  VkSubmitInfo submitInfo{};

  const VkSemaphore signalSemaphores[] = {
      synchronizer_->GetRenderFinishedSemaphores()[currentFrame_]};

  const VkSemaphore waitSemaphores[] = {
      synchronizer_->GetImageSemaphores()[currentFrame_]};

  constexpr VkPipelineStageFlags waitStages[] = {
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

  std::vector submittables = {cmdBuffer};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = waitSemaphores;
  submitInfo.pWaitDstStageMask = waitStages;
  submitInfo.commandBufferCount = submittables.size();
  submitInfo.pCommandBuffers = submittables.data();
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = signalSemaphores;

  // Submit queue
  VK_CHECK(vkQueueSubmit(logicalDevice_->GetGraphicQueue(), 1, &submitInfo,
                         synchronizer_->GetFences()[currentFrame_]));

  // PresentInfo
  VkPresentInfoKHR presentInfo{};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = signalSemaphores;

  // SwapChains
  const VkSwapchainKHR swapChains[] = {swapChain_->Get()};
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = swapChains;
  presentInfo.pImageIndices = &imageIndex_;

  vkQueuePresentKHR(logicalDevice_->GetGraphicQueue(), &presentInfo);

  currentFrame_ = (currentFrame_ + 1) % CONCURRENT_FRAMES_IN_FLIGHT;
}

void VulkanRenderer::UpdateInstance(const flecs::entity e) {

  const auto translate = glm::translate(
      glm::mat4(1.0f), e.has<ECS::Components::Position>()
                           ? e.get_ref<ECS::Components::Position>()->pos
                           : glm::vec3(0.0f));
  const auto scaling = glm::scale(
      glm::mat4(1.0f), e.has<ECS::Components::Dimension>()
                           ? e.get_ref<ECS::Components::Dimension>()->scale
                           : glm::vec3(1.0f));
  const auto rotation = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f),
                                    glm::vec3(0.0f, 0.0f, 1.0f));

  void* objectData;
  vmaMapMemory(allocator_->Get(), instanceData_->GetVmaAllocation(),
               &objectData);


  auto* object = static_cast<InstanceData*>(objectData);
  object[objectIndex_].model = (translate * scaling * rotation);
  object[objectIndex_].textureIndex =
      e.has<ECS::Components::Texture>()
          ? e.get_ref<ECS::Components::Texture>()->index
          : 0;
  vmaUnmapMemory(allocator_->Get(), instanceData_->GetVmaAllocation());
}
