#include "index_buffer.h"

namespace Entropy::Graphics::Vulkan::Buffers {

template <class T> IndexBuffer<T>::IndexBuffer() {
  // const VkDeviceSize bufferSize = sizeof(T) * indices.size();
  // assert(bufferSize != 0);
  CreateBuffer(1000, VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                         VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
  // vmaMapMemory(allocator_->Get(), allocation_, &mappedMemory_);
  // memcpy(mappedMemory_, indices.data(), bufferSize);
  // vmaUnmapMemory(allocator_->Get(), allocation_);
}

template <class T> IndexBuffer<T>::IndexBuffer(std::vector<T> indices) {
  const VkDeviceSize bufferSize = sizeof(T) * indices.size();
  assert(bufferSize != 0);
  CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT |
                               VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
  vmaMapMemory(allocator_->Get(), allocation_, &mappedMemory_);
  memcpy(mappedMemory_, indices.data(), bufferSize);
  vmaUnmapMemory(allocator_->Get(), allocation_);
}

// Explicit template instantiations
template class IndexBuffer<uint16_t>;
template class IndexBuffer<uint32_t>;

} // namespace Entropy::Graphics::Vulkan::Buffers
