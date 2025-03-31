#include "staging_buffer.h"

using namespace Entropy::Graphics::Vulkan::Buffers;

StagingBuffer::StagingBuffer(const VkDeviceSize size, const uint8_t *data,
                             const VkBufferUsageFlags flags) {
  CreateBuffer(size, flags);
  vmaMapMemory(allocator_->Get(), allocation_, &mappedMemory_);
  if (data != nullptr) {
    memcpy(mappedMemory_, data, size);
  }
  vmaUnmapMemory(allocator_->Get(), allocation_);
}
