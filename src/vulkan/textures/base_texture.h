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

#ifndef ENTROPY_VULKAN_BASE_TEXTURE_H
#define ENTROPY_VULKAN_BASE_TEXTURE_H

#include "vulkan/buffers/staging_buffer.h"
#include "vulkan/commandbuffers/commandbuffer.h"
#include "vulkan/descriptorsetlayouts/descriptorset_layout.h"
#include "vulkan/descriptorsets/descriptorset.h"
#include "vulkan/devices/ilogical_device.h"
#include "vulkan/devices/iphysical_device.h"
#include "vulkan/imageviews/imageview.h"
#include "vulkan/memory/allocator.h"

using namespace Entropy::Graphics::Vulkan::Buffers;
using namespace Entropy::Graphics::Vulkan::CommandBuffers;
using namespace Entropy::Graphics::Vulkan::ImageViews;
using namespace Entropy::Graphics::Vulkan::DescriptorSetLayouts;
using namespace Entropy::Graphics::Vulkan::DescriptorSets;

namespace Entropy::Graphics::Vulkan::Textures {

class BaseTexture {

public:
  std::shared_ptr<ImageView> GetImageView() { return imageView_; }
  std::shared_ptr<DescriptorSet> GetDescriptorSet() { return descriptorSet_; }
  [[nodiscard]] VkSampler GetSampler() const { return textureSampler_; }

protected:
  BaseTexture();
  ~BaseTexture();

  void TransitionImageLayout(VkImage image, VkImageLayout oldLayout,
                             VkImageLayout newLayout);

  void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width,
                         uint32_t height);

  void CreateImage(uint32_t width, uint32_t height, VkFormat format,
                   VkImageTiling tiling, VkImageUsageFlags usage,
                   VkImage &image);

  std::unique_ptr<CommandBuffer> commandBuffer_;
  std::unique_ptr<StagingBuffer> stagingBuffer_;
  std::shared_ptr<DescriptorSetLayout> descriptorSetLayout_;
  std::shared_ptr<DescriptorSet> descriptorSet_;
  std::shared_ptr<ImageView> imageView_;
  std::shared_ptr<Memory::IAllocator> allocator_;
  std::shared_ptr<Devices::IPhysicalDevice> physicalDevice_;
  std::shared_ptr<Devices::ILogicalDevice> logicalDevice_;

  VkImage textureImage_ = VK_NULL_HANDLE;
  VmaAllocation allocation_ = VK_NULL_HANDLE;
  VkSampler textureSampler_ = VK_NULL_HANDLE;
};
} // namespace Entropy::Graphics::Vulkan::Textures

#endif // ENTROPY_VULKAN_BASE_TEXTURE_H
