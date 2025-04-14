#include "storage_buffer.h"

using namespace Entropy::Vulkan::Buffers;

StorageBuffer::StorageBuffer(const VkDeviceSize bufferSize) {
  CreateBuffer(bufferSize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
               VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
                   VMA_ALLOCATION_CREATE_MAPPED_BIT);
}
