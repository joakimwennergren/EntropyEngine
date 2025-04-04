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

#ifndef ENTROPY_VULKAN_IALLOCATOR_H
#define ENTROPY_VULKAN_IALLOCATOR_H

#include <vk_mem_alloc.h>
#include "servicelocators/servicelocator.h"

namespace Entropy::Graphics::Vulkan::Memory {
/**
 * @class IAllocator
 * @brief Abstract interface for a memory allocator service.
 *
 * The IAllocator class provides an interface for a Vulkan Memory Allocator
 * (VmaAllocator) to be implemented by derived classes. It extends the IService
 * interface, indicating that it is part of a broader service system.
 *
 * This interface ensures that any allocator implementation provides access
 * to a VmaAllocator object via the Get() method.
 */
class IAllocator : public IService {
public:
  ~IAllocator() override = default;
  virtual VmaAllocator Get() = 0;
};
} // namespace Entropy::Graphics::Vulkan::Memory

#endif // ENTROPY_VULKAN_IALLOCATOR_H
