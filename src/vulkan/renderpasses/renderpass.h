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

#ifndef ENTROPY_VULKAN_RENDERPASS_H
#define ENTROPY_VULKAN_RENDERPASS_H

#include <vulkan/vulkan.hpp>

#include "vulkan/commandbuffers/commandbuffer.h"
#include "vulkan/devices/logical_device.h"
#include "vulkan/swapchains/swapchain.h"
#include "vulkan/textures/depthbuffer_texture.h"
#include "vulkan/textures/swapchain_texture.h"

namespace Entropy::Graphics::Vulkan::RenderPasses {
class RenderPass {
public:
  RenderPass();
  ~RenderPass();

  void Begin(const std::shared_ptr<CommandBuffers::CommandBuffer> &commandBuffer,
             uint32_t imageIndex);

  static void End(std::shared_ptr<CommandBuffers::CommandBuffer> &commandBuffer);

  void CreateFrameBuffers(uint32_t width, uint32_t height);
  void RecreateFrameBuffers(uint32_t width, uint32_t height);

  void RecreateDepthBuffer(uint32_t width, uint32_t height);

  [[nodiscard]] VkRenderPass Get() const { return renderPass_; }

private:
  [[nodiscard]] VkFormat
  findSupportedFormat(const std::vector<VkFormat> &candidates,
                      VkImageTiling tiling, VkFormatFeatureFlags features);

  [[nodiscard]] VkFormat FindDepthFormat();

  VkRenderPass renderPass_ = VK_NULL_HANDLE;
  std::vector<VkFramebuffer> frameBuffers_;
  std::vector<std::shared_ptr<Textures::SwapChainTexture>> swapChainTextures_;
  std::shared_ptr<Textures::DepthBufferTexture> depthBufferTexture_;
  std::shared_ptr<Devices::ILogicalDevice> logicalDevice_;
  std::shared_ptr<Devices::IPhysicalDevice> physicalDevice_;
  std::shared_ptr<SwapChains::ISwapChain> swapChain_;
};
} // namespace Entropy::Graphics::Vulkan::RenderPasses

#endif // ENTROPY_VULKAN_RENDERPASS_H