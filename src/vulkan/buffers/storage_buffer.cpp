#include "storage_buffer.h"

using namespace Entropy::Graphics::Vulkan::Buffers;

StorageBuffer::StorageBuffer(const VkDeviceSize bufferSize) {
  CreateBuffer(bufferSize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
  vmaMapMemory(allocator_->Get(), allocation_, &mappedMemory_);
  vmaUnmapMemory(allocator_->Get(), allocation_);
}
