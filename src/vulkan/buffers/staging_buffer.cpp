#include "staging_buffer.h"

using namespace Entropy::Graphics::Vulkan::Buffers;

StagingBuffer::StagingBuffer(const VkDeviceSize size, const uint8_t *data,
                             const VkBufferUsageFlags flags) {
  CreateBuffer(size, flags, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
                      VMA_ALLOCATION_CREATE_MAPPED_BIT);
  if (data != nullptr) {
    memcpy(allocationInfo_.pMappedData, data, size);
  }
  //vmaUnmapMemory(allocator_->Get(), allocation_);
}
