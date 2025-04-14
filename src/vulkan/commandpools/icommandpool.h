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

#ifndef ENTROPY_VULKAN_ICOMMANDPOOL_H
#define ENTROPY_VULKAN_ICOMMANDPOOL_H

#include <vulkan/vulkan.hpp>
#include "servicelocators/servicelocator.h"

namespace Entropy::Vulkan::CommandPools {

/**
 * @class ICommandPool
 * @brief Interface that defines a pool for managing and reusing ICommand
 * objects.
 *
 * ICommandPool provides a mechanism for efficiently managing the lifecycle of
 * command objects by pooling and reusing them. Implementations of this
 * interface are responsible for creating, resetting, and recycling ICommand
 * instances.
 *
 * The pool helps to minimize the overhead of repeatedly creating and destroying
 * ICommand objects, improving performance and reducing memory fragmentation.
 *
 * @note Implementers should ensure thread-safety if the pool is to be accessed
 *       from multiple threads simultaneously.
 */
class ICommandPool : public IService {
 public:
  ~ICommandPool() override = default;
  virtual VkCommandPool Get() = 0;
};

}  // namespace Entropy::Vulkan::CommandPools

#endif  // ENTROPY_VULKAN_ICOMMANDPOOL_H
