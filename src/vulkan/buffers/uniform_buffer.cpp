#include "uniform_buffer.h"

using namespace Entropy::Graphics::Vulkan::Buffers;

UniformBuffer::UniformBuffer(const VkDeviceSize bufferSize) {
  assert(bufferSize != 0);
  CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
  vmaMapMemory(allocator_->Get(), allocation_, &mappedMemory_);
  vmaUnmapMemory(allocator_->Get(), allocation_);
}
