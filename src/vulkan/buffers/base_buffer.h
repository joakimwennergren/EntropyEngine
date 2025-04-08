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

#ifndef ENTROPY_VULKAN_BASE_BUFFER_H
#define ENTROPY_VULKAN_BASE_BUFFER_H

#include <vulkan/vulkan.hpp>

#include "vulkan/devices/ilogical_device.h"
#include "vulkan/memory/allocator.h"

namespace Entropy::Graphics::Vulkan::Buffers {

/**
 * @class BaseBuffer
 * @brief A base class for handling buffer-related operations.
 *
 * The BaseBuffer class serves as a fundamental structure for managing
 * data stored in a contiguous memory block. It provides basic functionalities
 * that can be extended for specific buffer types, such as resizing,
 * clearing, and data management.
 *
 * This class is intended to be used as a base class for derived buffer
 * classes that implement specific behaviors.
 */
class BaseBuffer {
public:
  ~BaseBuffer();
  [[nodiscard]] VkBuffer GetVulkanBuffer() const { return buffer_; };
  [[nodiscard]] void *GetMappedMemory() const { return allocationInfo_.pMappedData; };
  [[nodiscard]] VmaAllocation GetVmaAllocation() const { return allocation_; };

protected:
  void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VmaAllocationCreateFlags flags);

  VkBuffer buffer_ = VK_NULL_HANDLE;
  VkDeviceMemory bufferMemory_ = VK_NULL_HANDLE;
  VmaAllocation allocation_ = VK_NULL_HANDLE;
  VmaAllocationInfo allocationInfo_{};
  std::shared_ptr<Memory::IAllocator> allocator_;
  std::shared_ptr<Devices::ILogicalDevice> logicalDevice_;
};
} // namespace Entropy::Graphics::Vulkan::Buffers

#endif // ENTROPY_VULKAN_BASE_BUFFER_H
