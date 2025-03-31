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

#ifndef ENTROPY_VULKAN_STAGING_BUFFER_H
#define ENTROPY_VULKAN_STAGING_BUFFER_H

#include "base_buffer.h"

namespace Entropy::Graphics::Vulkan::Buffers {

/**
 * @class StagingBuffer
 *
 * @brief Represents a memory buffer used for temporary data storage during data
 * transfer operations, such as copying data between a host and a GPU or between
 * different GPU memory regions.
 *
 * This class is designed to optimize data transfer workflows by providing
 * an intermediate staging area, enabling efficient movement of data.
 * StagingBuffer typically resides in a memory region suited for fast access by
 * both the CPU and GPU.
 *
 * Key features of the StagingBuffer include:
 * - Allocation of memory optimized for staging purposes.
 * - Methods to map and unmap the buffer for CPU access.
 * - Support for data synchronization between memory regions.
 *
 * StagingBuffer is commonly used in graphics and compute applications
 * where frequent data updates are required between CPU and GPU memory.
 */
class StagingBuffer final : public BaseBuffer {
public:
  StagingBuffer(VkDeviceSize size, const uint8_t *data,
                VkBufferUsageFlags flags);
};
} // namespace Entropy::Graphics::Vulkan::Buffers

#endif // ENTROPY_VULKAN_STAGING_BUFFER_H
