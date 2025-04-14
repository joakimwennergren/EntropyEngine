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

#ifndef ENTROPY_VULKAN_COMMANDPOOL_H
#define ENTROPY_VULKAN_COMMANDPOOL_H

#include <vulkan/vulkan.hpp>
#include "icommandpool.h"
#include "vulkan/devices/ilogical_device.h"

namespace Entropy::Vulkan::CommandPools {

/**
 * @class CommandPool
 * @brief Manages the allocation and lifetime of command buffers in a Vulkan
 * application.
 *
 * The CommandPool class encapsulates functionality for creating and managing
 * Vulkan command pools. It is responsible for allocating and freeing command
 * buffers and serves as an abstraction layer to simplify Vulkan resource
 * management.
 *
 * A command pool is associated with a specific Vulkan device and queue family.
 * Command buffers allocated from a command pool inherit the capabilities of
 * the queue family it was created for. The pool is designed to be reset or
 * entirely destroyed along with its associated command buffers.
 *
 * Ownership and cleanup of command buffers are tied to this pool, allowing for
 * efficient batch resets and lifetime control.
 *
 * This class provides:
 * - Creation and destruction of a Vulkan command pool.
 * - Allocation of command buffers from the pool.
 * - Resetting the command pool and releasing resources.
 *
 */
class CommandPool final : public ServiceBase<ICommandPool> {
 public:
  CommandPool();
  ~CommandPool() override;
  VkCommandPool Get() override { return commandPool_; }

 private:
  VkCommandPool commandPool_ = VK_NULL_HANDLE;
  std::shared_ptr<Devices::ILogicalDevice> logicalDevice_;
};

}  // namespace Entropy::Vulkan::CommandPools

#endif  // ENTROPY_VULKAN_COMMANDPOOL_H
