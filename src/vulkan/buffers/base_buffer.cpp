#include "base_buffer.h"
#include "servicelocators/servicelocator.h"
#include "vulkan/utilities/helpers.h"
#include "vulkan/devices/ilogical_device.h"

using namespace Entropy::Graphics::Vulkan::Buffers;

BaseBuffer::~BaseBuffer() {
  vkDeviceWaitIdle(logicalDevice_->Get());
  vmaDestroyBuffer(allocator_->Get(), buffer_, allocation_);
}

void BaseBuffer::CreateBuffer(const VkDeviceSize size,
                              const VkBufferUsageFlags usage) {
  const ServiceLocator *sl = ServiceLocator::GetInstance();
  allocator_ = sl->getService<Memory::IAllocator>();
  logicalDevice_ = sl->getService<Devices::ILogicalDevice>();

  assert(size != 0);
  assert(usage != 0);

  VkBufferCreateInfo bufferInfo = {};
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = size;
  bufferInfo.usage = usage;

  VmaAllocationCreateInfo allocInfo = {};
  allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
  allocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT;

  VK_CHECK(vmaCreateBuffer(allocator_->Get(), &bufferInfo, &allocInfo, &buffer_,
                           &allocation_, nullptr));
}