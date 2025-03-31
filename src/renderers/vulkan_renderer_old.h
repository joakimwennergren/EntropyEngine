// Copyright 2025 Joakim Wennergren
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef ENTROPY_VULKAN_RENDERER_H
#define ENTROPY_VULKAN_RENDERER_H

#include <ecs/components/label.hpp>

#include "cameras/camera_manager.hpp"
#include "config.hpp"
#include "ecs/components/position.hpp"
#include "graphics/vulkan/memory/allocator.hpp"
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <flecs.h>

#include <cameras/orthographic_camera.hpp>
#include <data/ubo.hpp>
#include <ecs/components/animated_sprite.hpp>
#include <ecs/components/renderable.hpp>
#include <ecs/world.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <graphics/vulkan/buffers/stagingbuffer.hpp>
#include <graphics/vulkan/buffers/storagebuffer.hpp>
#include <graphics/vulkan/buffers/uniformbuffer.hpp>
#include <graphics/vulkan/commandbuffers/commandbuffer.hpp>
#include <graphics/vulkan/pipelines/static_pipeline.hpp>
#include <graphics/vulkan/renderpasses/renderpass.hpp>
#include <graphics/vulkan/swapchains/swapchain.hpp>
#include <graphics/vulkan/synchronization/synchronizer.hpp>
#include <obj/model.hpp>
#include <timing/timer.hpp>
// #include <tracy/Tracy.hpp>

#ifdef BUILD_FOR_ANDROID
#include <android/asset_manager.h>
#endif

using namespace Entropy::Graphics::Vulkan::Buffers;
using namespace Entropy::Graphics::Vulkan::Textures;
using namespace Entropy::Graphics::Vulkan::Pipelines;
using namespace Entropy::Graphics::Vulkan::RenderPasses;
using namespace Entropy::Graphics::Vulkan::CommandBuffers;
using namespace Entropy::Graphics::Vulkan::Synchronization;
using namespace Entropy::Graphics::Vulkan::DescriptorSets;
using namespace Entropy::Graphics::Vulkan::SwapChains;
using namespace Entropy::Graphics::Vulkan::Synchronization;
using namespace Entropy::Data;
using namespace Entropy::ECS;
using namespace Entropy::Cameras;

namespace Entropy::Graphics::Vulkan::Renderers {
struct VulkanRenderer {
  VulkanRenderer() {
    const ServiceLocator *sl = ServiceLocator::GetInstance();

    _allocator = sl->getService<IAllocator>();
    _logicalDevice = sl->getService<ILogicalDevice>();
    _swapchain = sl->getService<ISwapchain>();
    _world = sl->getService<IWorld>();
    _cameraManager = sl->getService<ICameraManager>();

    _renderPass = std::make_shared<RenderPass>();
    _synchronizer =
        std::make_unique<Synchronizer>(MAX_CONCURRENT_FRAMES_IN_FLIGHT);
    //_renderPass->CreateFrameBuffers(800, 800);

    // Static Pipeline creation
    _staticPipeline = std::make_unique<StaticPipeline>(_renderPass);

    for (uint32_t i = 0; i < MAX_CONCURRENT_FRAMES_IN_FLIGHT; i++) {
      auto buf =
          std::make_shared<CommandBuffer>(VK_COMMAND_BUFFER_LEVEL_PRIMARY);
      _commandBuffers.emplace_back(buf);
    }

    _UBO = std::make_unique<UniformBuffer>(sizeof(UboDataDynamic));
    _instanceDataBuffer = std::make_unique<StorageBuffer>(MAX_INSTANCE_COUNT *
                                                          sizeof(InstanceData));



    // Create a query for the Position component with a custom sorting function
    _sortQuery = _world->Get()
                     ->query_builder<Components::Position>()
                     .order_by<Components::Position>(compare_zIndex)
                     .build();

    _timer = std::make_unique<Timing::Timer>(1.0f);
    _timer->Start();
  }

  ~VulkanRenderer() = default;

  void Render(int width, int height, float xscale, float yscale);

  // Comparison function for sorting based on z index
  static int compare_zIndex(flecs::entity_t e1, const Components::Position *p1,
                            flecs::entity_t e2,
                            const Components::Position *p2) {
    return (p1->pos.z > p2->pos.z) - (p1->pos.z < p2->pos.z);
  }

  void PresentForApplication() {
    // End render pass and command buffer recording
    RenderPass::End(_commandBuffers[_currentFrame]);
    _commandBuffers[_currentFrame]->EndRecording();

    const auto cmdBuffer = _commandBuffers[_currentFrame]->Get();

    // Submit info
    VkSubmitInfo submitInfo{};
    const VkSemaphore signalSemaphores[] = {
        _synchronizer->GetRenderFinishedSemaphores()[_currentFrame]};
    const VkSemaphore waitSemaphores[] = {
        _synchronizer->GetImageSemaphores()[_currentFrame]};
    VkPipelineStageFlags waitStages[] = {
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
    VK_CHECK(vkQueueSubmit(_logicalDevice->GetGraphicQueue(), 1, &submitInfo,
                           _synchronizer->GetFences()[_currentFrame]));

    // PresentInfo
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    // SwapChains
    const VkSwapchainKHR swapChains[] = {_swapchain->Get()};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    // Present
    vkQueuePresentKHR(_logicalDevice->GetGraphicQueue(), &presentInfo);
  }

  void RenderEntity(flecs::entity e, uint32_t width, uint32_t height) {
    auto position_component = e.get_ref<Components::Position>();
    auto scale_component = e.get_ref<Components::Dimension>();
    auto color_component = e.get_ref<Components::Color>();
    auto rotation_component = e.get_ref<Components::Rotation>();
    auto render_component = e.get_ref<Components::Renderable>();

    if (render_component->visible == false) return;

    auto translate = glm::mat4(1.0f);
    auto rotation = glm::mat4(1.0f);
    auto scaling = glm::mat4(1.0f);

    if (position_component.get() != nullptr) {
      translate = glm::translate(glm::mat4(1.0f), position_component->pos);
    }

    if (scale_component.get() != nullptr) {
      scaling = scale(glm::mat4(1.0f), scale_component->scale);
    }

    if (rotation_component.get() != nullptr) {
      rotation =
          rotate(glm::mat4(1.0f), glm::radians(rotation_component->angle),
                 rotation_component->orientation);
    }

    void *objectData;
    vmaMapMemory(_allocator->Get(), _instanceDataBuffer->allocation,
                 &objectData);

    auto *objectSSBO = static_cast<InstanceData *>(objectData);
    objectSSBO[objectIndex].model = (translate * scaling * rotation);
    objectSSBO[objectIndex].bgColor = color_component->color;
    objectSSBO[objectIndex].borderColor = color_component->borderColor;
    objectSSBO[objectIndex].cornerRadiuses = scale_component->cornerRadiuses;
    objectSSBO[objectIndex].type = render_component->type;
    objectSSBO[objectIndex].dimension = scale_component->scale;

    vmaUnmapMemory(_allocator->Get(), _instanceDataBuffer->allocation);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = {width, height};
    vkCmdSetScissor(_commandBuffers[_currentFrame]->Get(), 0, 1, &scissor);

    // Set Viewport
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(width);
    viewport.height = static_cast<float>(height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(_commandBuffers[_currentFrame]->Get(), 0, 1, &viewport);

    vkCmdBindPipeline(_commandBuffers[_currentFrame]->Get(),
                      VK_PIPELINE_BIND_POINT_GRAPHICS,
                      _staticPipeline->GetPipeline());

    const auto ds0 = _staticPipeline->descriptorSet;

    vkCmdBindDescriptorSets(
        _commandBuffers[_currentFrame]->Get(), VK_PIPELINE_BIND_POINT_GRAPHICS,
        _staticPipeline->GetPipelineLayout(), 0, 1, &ds0, 0, nullptr);

    if (e.has<Components::HasTexture>()) {
      const auto texture = e.get<Components::HasTexture>();
      if (texture->texture->imageView != nullptr) {
        vkCmdBindDescriptorSets(_commandBuffers[_currentFrame]->Get(),
                                VK_PIPELINE_BIND_POINT_GRAPHICS,
                                _staticPipeline->GetPipelineLayout(), 1, 1,
                                &texture->texture->descriptorSet, 0, nullptr);
      }
    }

    if (e.has<Components::HasAnimatedSprite>()) {
      const auto textures = e.get<Components::HasAnimatedSprite>();
      static int textureId;

      if (_timer->GetTick() >= 120.0) {
        textureId = (textureId + 1) % textures->textures.size();
        _timer->Reset();
      }

      vkCmdBindDescriptorSets(
          _commandBuffers[_currentFrame]->Get(),
          VK_PIPELINE_BIND_POINT_GRAPHICS, _staticPipeline->GetPipelineLayout(),
          1, 1, &textures->textures[textureId]->descriptorSet, 0, nullptr);
    }

    PushConstBlock constants{};
    constants.instanceIndex = objectIndex;

    // upload the matrix to the GPU via push constants
    vkCmdPushConstants(_commandBuffers[_currentFrame]->Get(),
                       _staticPipeline->GetPipelineLayout(),
                       VK_SHADER_STAGE_ALL, 0, sizeof(PushConstBlock),
                       &constants);

    if (render_component.get() != nullptr &&
        e.get<Components::Renderable>()->indexBuffer == nullptr &&
        e.get<Components::Renderable>()->vertexBuffer != nullptr) {
      const auto renderable = e.get<Components::Renderable>();
      // Bind vertex & index buffers
      const VkBuffer vertexBuffers[] = {
          renderable->vertexBuffer->GetVulkanBuffer()};
      constexpr VkDeviceSize offsets[] = {0};
      vkCmdBindVertexBuffers(_commandBuffers[_currentFrame]->Get(), 0, 1,
                             vertexBuffers, offsets);

      vkCmdDraw(_commandBuffers[_currentFrame]->Get(),
                renderable->vertices.size(), 1, 0, 0);
    }

    if (render_component.get() != nullptr &&
        e.get<Components::Renderable>()->indexBuffer != nullptr &&
        e.get<Components::Renderable>()->vertexBuffer != nullptr) {
      const auto renderable = e.get<Components::Renderable>();
      // Bind vertex & index buffers
      const VkBuffer vertexBuffers[] = {
          renderable->vertexBuffer->GetVulkanBuffer()};
      const VkDeviceSize offsets[] = {0};
      vkCmdBindVertexBuffers(_commandBuffers[_currentFrame]->Get(), 0, 1,
                             vertexBuffers, offsets);

      vkCmdBindIndexBuffer(_commandBuffers[_currentFrame]->Get(),
                           renderable->indexBuffer->GetVulkanBuffer(), 0,
                           VK_INDEX_TYPE_UINT16);

      // Draw current renderable
      vkCmdDrawIndexed(_commandBuffers[_currentFrame]->Get(),
                       renderable->indices.size(), 1, 0, 0, 0);
    }
    objectIndex++;
  }

  void OnResize(const int width, const int height) {
    _synchronizer =
        std::make_unique<Synchronizer>(MAX_CONCURRENT_FRAMES_IN_FLIGHT);
    _swapchain->RecreateSwapChain(width, height);
    _renderPass->RecreateDepthBuffer(width, height);
    _renderPass->RecreateFrameBuffers(width, height);
  }

  std::shared_ptr<RenderPass> _renderPass;
  std::shared_ptr<ISwapchain> _swapchain;

 private:
  flecs::query<Components::Position> _sortQuery;

  // Container to hold entities and their Position components
  std::vector<flecs::entity> _sortedEntities;

  // Custom comparator function for sorting
  static bool CompareZIndex(const flecs::entity &a, const flecs::entity &b) {
    return a.get_ref<Components::Position>()->pos.z <
           b.get_ref<Components::Position>()->pos.z;
    // Sort by zIndex
  }

  std::unique_ptr<Timing::Timer> _timer;

  uint32_t _currentFrame = 0;
  uint32_t imageIndex = 0;

  // Command Buffers
  std::vector<std::shared_ptr<CommandBuffer>> _commandBuffers;

  uint32_t objectIndex = 0;

  // UBO
  std::unique_ptr<UniformBuffer> _UBO;

  std::unique_ptr<StorageBuffer> _instanceDataBuffer;
  // Pipelines
  std::unique_ptr<StaticPipeline> _staticPipeline;
  // Synchronizer
  std::unique_ptr<Synchronizer> _synchronizer;

  // Dependencies
  std::shared_ptr<IAllocator> _allocator;
  std::shared_ptr<ILogicalDevice> _logicalDevice;
  std::shared_ptr<IWorld> _world;
  std::shared_ptr<ICameraManager> _cameraManager;
};
}  // namespace Entropy::Graphics::Vulkan::Renderers

#endif  // ENTROPY_VULKAN_RENDERER_H