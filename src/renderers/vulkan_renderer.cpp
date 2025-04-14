#include "vulkan_renderer.h"
#include <glm/glm.hpp>
#include "vulkan/data/data.h"

using namespace Entropy::Vulkan::Memory;
using namespace Entropy::Vulkan::Devices;
using namespace Entropy::Vulkan::SwapChains;
using namespace Entropy::Vulkan::Synchronization;
using namespace Entropy::Vulkan::Pipelines;
using namespace Entropy::Vulkan::RenderPasses;
using namespace Entropy::Vulkan::Buffers;
using namespace Entropy::Vulkan::Data;
using namespace Entropy::Cameras;
using namespace Entropy::Assets;
using namespace Entropy::Renderers;

VulkanRenderer::VulkanRenderer(const uint32_t width, const uint32_t height)
    : frame_(nullptr) {
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

  instanceDataBuffer_ = std::make_unique<VertexBuffer<InstanceDataTwoD>>(
      MAX_INSTANCE_COUNT * sizeof(InstanceDataTwoD));
  indexDataBuffer_ =
      std::make_unique<IndexBuffer<uint16_t>>(MAX_INSTANCE_COUNT);

  frame_ = new FrameData<TwoDVertex, uint16_t, InstanceDataTwoD>();

  std::array<VkWriteDescriptorSet, 1> descriptorWrites{};

  VkDescriptorBufferInfo uboInfo{};
  uboInfo.buffer = UBO_->GetVulkanBuffer();
  uboInfo.offset = 0;
  uboInfo.range = sizeof(UBOData);

  descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  descriptorWrites[0].dstSet = two_d_pipeline_->descriptorSet;
  descriptorWrites[0].dstBinding = 1;
  descriptorWrites[0].dstArrayElement = 0;
  descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  descriptorWrites[0].descriptorCount = 1;
  descriptorWrites[0].pBufferInfo = &uboInfo;

  vkUpdateDescriptorSets(logicalDevice_->Get(), descriptorWrites.size(),
                         descriptorWrites.data(), 0, nullptr);
}

VulkanRenderer::~VulkanRenderer() {
  //delete frame_;
}

void VulkanRenderer::Resize(const uint32_t width, const uint32_t height) {
  synchronizer_ = std::make_unique<Synchronizer>(CONCURRENT_FRAMES_IN_FLIGHT);
  swapChain_->RecreateSwapChain(width, height);
  renderPass_->RecreateDepthBuffer(width, height);
  renderPass_->RecreateFrameBuffers(width, height);
}

uint32_t VulkanRenderer::Frame(
    FrameData<TwoDVertex, uint16_t, InstanceDataTwoD>* frame) {
  frame_ = frame;
  return frame_->data.size();
}

FrameData<TwoDVertex, uint16_t, InstanceDataTwoD>* VulkanRenderer::GetFrame() {
  return frame_;
}

void VulkanRenderer::End() {
  frame_->indices.clear();
  frame_->vertices.clear();
  frame_->data.clear();
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

  const UBOData ubo{camera->projection, camera->view};
  memcpy(UBO_->GetMappedMemory(), &ubo, sizeof(ubo));

  commandBuffers_[currentFrame_]->Record();
  renderPass_->Begin(commandBuffers_[currentFrame_], imageIndex_);

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = {width, height};
  vkCmdSetScissor(commandBuffers_[currentFrame_]->Get(), 0, 1, &scissor);

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

  vertexDataBuffer_->Update(frame_->vertices);
  indexDataBuffer_->Update(frame_->indices);
  instanceDataBuffer_->Update(frame_->data);

  const auto vertexBuffer = vertexDataBuffer_->GetVulkanBuffer();
  const auto instanceBuffer = instanceDataBuffer_->GetVulkanBuffer();
  constexpr VkDeviceSize offsets[1] = {0};
  vkCmdBindVertexBuffers(commandBuffers_[currentFrame_]->Get(), 0, 1,
                         &vertexBuffer, offsets);
  vkCmdBindVertexBuffers(commandBuffers_[currentFrame_]->Get(), 1, 1,
                         &instanceBuffer, offsets);
  vkCmdBindIndexBuffer(commandBuffers_[currentFrame_]->Get(),
                       indexDataBuffer_->GetVulkanBuffer(), 0,
                       VK_INDEX_TYPE_UINT16);

  vkCmdDrawIndexed(commandBuffers_[currentFrame_]->Get(),
                   frame_->indices.size(), frame_->data.size(), 0, 0, 0);

  RenderPass::End(commandBuffers_[currentFrame_]);
  commandBuffers_[currentFrame_]->EndRecording();

  constexpr VkPipelineStageFlags waitStages[1] = {
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
  VK_CHECK(vkQueuePresentKHR(logicalDevice_->GetGraphicQueue(), &presentInfo));
  currentFrame_ = (currentFrame_ + 1) % CONCURRENT_FRAMES_IN_FLIGHT;
}