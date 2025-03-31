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

#ifndef ENTROPY_VULKAN_ALLOCATOR_H
#define ENTROPY_VULKAN_ALLOCATOR_H

#include "iallocator.h"

namespace Entropy::Graphics::Vulkan::Memory {

/**
 * @class Allocator
 * @brief A final class responsible for managing Vulkan Memory Allocator (VMA)
 * resources.
 *
 * Allocator handles the setup and management of Vulkan Memory Allocator
 * instances, providing a streamlined way to allocate and manage Vulkan-specific
 * memory in conjunction with Vulkan objects such as devices, physical devices,
 * and instances.
 *
 * This class is a concrete implementation of the IAllocator interface and
 * inherits from the ServiceBase class.
 *
 * The Vulkan Memory Allocator (VMA) is initialized and maintained internally,
 * ensuring proper cleanup during destruction. It exposes the allocator instance
 * for use via the `Get` method.
 *
 * The class is non-copyable due to its management of underlying Vulkan handles.
 */
class Allocator final : public ServiceBase<IAllocator> {
public:
  Allocator();
  ~Allocator() override;
  VmaAllocator Get() override { return _allocator; }

private:
  VmaAllocator _allocator = VK_NULL_HANDLE;
};
} // namespace Entropy::Graphics::Vulkan::Memory

#endif // ENTROPY_VULKAN_ALLOCATOR_H
