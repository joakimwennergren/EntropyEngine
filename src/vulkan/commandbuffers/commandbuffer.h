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

#ifndef ENTROPY_VULKAN_COMMAND_BUFFER_H
#define ENTROPY_VULKAN_COMMAND_BUFFER_H

#include <vulkan/vulkan.hpp>

#include "vulkan/commandpools/icommandpool.h"
#include "vulkan/devices/ilogical_device.h"

namespace Entropy::Graphics::Vulkan::CommandBuffers {

/**
 * @class CommandBuffer
 * @brief A class that manages and records a sequence of API commands for
 * execution.
 *
 * The CommandBuffer class is responsible for collecting and organizing
 * commands, which can be later executed in a sequential and efficient manner.
 * This is typically used in rendering or computational tasks where batch
 * processing of commands is desired. The commands are recorded in the buffer
 * and can then be submitted for processing by the appropriate execution unit.
 *
 * Key features include:
 * - Recording various commands into the buffer.
 * - Providing mechanisms to execute or reset the recorded commands.
 * - Managing the lifecycle and synchronization of commands for optimal
 * performance.
 *
 * This class ensures proper validation and optimization of commands before
 * execution.
 */
class CommandBuffer {
public:
  explicit CommandBuffer(VkCommandBufferLevel level);
  ~CommandBuffer();
  [[nodiscard]] VkCommandBuffer Get() const { return commandBuffer_; }

  void Record() const;
  void EndRecording() const;

  void RecordOnce() const;
  void EndRecordingOnce() const;

private:
  VkCommandBuffer commandBuffer_ = VK_NULL_HANDLE;
  std::shared_ptr<Devices::ILogicalDevice> logicalDevice_;
  std::shared_ptr<CommandPools::ICommandPool> commandPool_;
};
} // namespace Entropy::Graphics::Vulkan::CommandBuffers

#endif // ENTROPY_VULKAN_COMMAND_BUFFER_H
