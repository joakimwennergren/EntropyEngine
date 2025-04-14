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

#ifndef ENTROPY_VULKAN_BATCH_RENDERER_H
#define ENTROPY_VULKAN_BATCH_RENDERER_H

#include "irenderer.h"
#include "assets/iasset_manager.h"
#include "cameras/icamera_manger.h"
#include "ecs/iworld.h"
#include "vulkan/buffers/index_buffer.h"
#include "vulkan/buffers/uniform_buffer.h"
#include "vulkan/buffers/vertex_buffer.h"
#include "vulkan/commandbuffers/commandbuffer.h"
#include "vulkan/devices/ilogical_device.h"
#include "vulkan/memory/iallocator.h"
#include "vulkan/pipelines/twod_pipeline.h"
#include "vulkan/renderpasses/renderpass.h"
#include "vulkan/swapchains/iswapchain.h"
#include "vulkan/synchronization/synchronizer.h"

namespace Entropy::Renderers {
class VulkanRenderer final : public ServiceBase<IRenderer>{
 public:
  VulkanRenderer(uint32_t width, uint32_t height);
  ~VulkanRenderer() override;
  void Render(uint32_t width, uint32_t height) override;
  void Resize(uint32_t width, uint32_t height) override;
  uint32_t  Frame(Graphics::Vulkan::Data::FrameData<Graphics::Vulkan::Data::TwoDVertex,
    uint16_t,
    Graphics::Vulkan::Data::InstanceDataTwoD> *frame) override;
  Graphics::Vulkan::Data::FrameData<Graphics::Vulkan::Data::TwoDVertex,
    uint16_t,
    Graphics::Vulkan::Data::InstanceDataTwoD> *GetFrame() override;
  void End() override;

 private:
  uint32_t currentFrame_{};
  uint32_t imageIndex_{};

  Graphics::Vulkan::Data::FrameData<Graphics::Vulkan::Data::TwoDVertex, uint16_t,
    Graphics::Vulkan::Data::InstanceDataTwoD> *frame_{};

  std::vector<std::shared_ptr<CommandBuffer>> commandBuffers_;
  std::unique_ptr<UniformBuffer> UBO_;

  std::unique_ptr<VertexBuffer<Graphics::Vulkan::Data::TwoDVertex>>
      vertexDataBuffer_;

  std::unique_ptr<VertexBuffer<Graphics::Vulkan::Data::InstanceDataTwoD>>
    instanceDataBuffer_;
  std::unique_ptr<IndexBuffer<uint16_t>> indexDataBuffer_;
  std::unique_ptr<Graphics::Vulkan::Synchronization::Synchronizer>
      synchronizer_;

  std::shared_ptr<Graphics::Vulkan::RenderPasses::RenderPass> renderPass_;
  std::shared_ptr<ECS::IWorld> world_;
  std::shared_ptr<Graphics::Vulkan::Memory::IAllocator> allocator_;
  std::shared_ptr<Graphics::Vulkan::Devices::ILogicalDevice> logicalDevice_;
  std::shared_ptr<Graphics::Vulkan::SwapChains::ISwapChain> swapChain_;
  std::shared_ptr<Cameras::ICameraManager> cameraManager_;
};
}  // namespace Entropy::Vulkan::Renderers
#endif  // ENTROPY_VULKAN_BATCH_RENDERER_H
