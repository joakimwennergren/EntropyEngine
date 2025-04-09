#include "vertex_buffer.h"

namespace Entropy::Graphics::Vulkan::Buffers {

template <class T>
VertexBuffer<T>::VertexBuffer(const VkDeviceSize size) {
  assert(size != 0);
  CreateBuffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT |
  VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
VMA_ALLOCATION_CREATE_MAPPED_BIT);
}

template <class T>
void VertexBuffer<T>::Update(std::vector<T> vertices) {
  memcpy(allocationInfo_.pMappedData, vertices.data(), vertices.size() * sizeof(T));
}

// Explicit template instantiations
template class VertexBuffer<Data::TwoDVertex>;
template class VertexBuffer<Data::ThreeDAnimVertex>;
template class VertexBuffer<Data::InstanceData>;
} // namespace Entropy::Graphics::Vulkan::Buffers
