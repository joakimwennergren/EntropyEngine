#include "index_buffer.h"

namespace Entropy::Graphics::Vulkan::Buffers {

template <class T> IndexBuffer<T>::IndexBuffer(VkDeviceSize size) {
  assert(size != 0);
  CreateBuffer(size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
VMA_ALLOCATION_CREATE_MAPPED_BIT);
}

template <class T>
void IndexBuffer<T>::Update(std::vector<T> indices) {
  memcpy(allocationInfo_.pMappedData, indices.data(), indices.size() * sizeof(T));
}

// Explicit template instantiations
template class IndexBuffer<uint16_t>;
template class IndexBuffer<uint32_t>;

} // namespace Entropy::Graphics::Vulkan::Buffers
