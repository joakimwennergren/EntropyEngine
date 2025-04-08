#include "uniform_buffer.h"

using namespace Entropy::Graphics::Vulkan::Buffers;

UniformBuffer::UniformBuffer(const VkDeviceSize bufferSize) {
  assert(bufferSize != 0);
  CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
                      VMA_ALLOCATION_CREATE_MAPPED_BIT);
}
