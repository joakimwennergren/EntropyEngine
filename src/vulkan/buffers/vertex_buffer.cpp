#include "vertex_buffer.h"

namespace Entropy::Graphics::Vulkan::Buffers {
template <class T> VertexBuffer<T>::VertexBuffer() {
  // const VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
  // assert(bufferSize != 0);
  CreateBuffer(1000, VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                         VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
  // vmaMapMemory(allocator_->Get(), allocation_, &mappedMemory_);
  // memcpy(mappedMemory_, vertices.data(), bufferSize);
  // vmaUnmapMemory(allocator_->Get(), allocation_);
}

template <class T>
VertexBuffer<T>::VertexBuffer(const std::vector<T> &vertices) {
  const VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
  assert(bufferSize != 0);
  CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT |
                               VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
  vmaMapMemory(allocator_->Get(), allocation_, &mappedMemory_);
  memcpy(mappedMemory_, vertices.data(), bufferSize);
  vmaUnmapMemory(allocator_->Get(), allocation_);
}

// Explicit template instantiations
template class VertexBuffer<Data::TwoDVertex>;
template class VertexBuffer<Data::ThreeDAnimVertex>;
} // namespace Entropy::Graphics::Vulkan::Buffers
