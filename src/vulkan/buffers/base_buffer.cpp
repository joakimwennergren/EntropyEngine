#include "base_buffer.h"
#include "servicelocators/servicelocator.h"
#include "vulkan/devices/ilogical_device.h"
#include "vulkan/utilities/helpers.h"

using namespace Entropy::Vulkan::Buffers;

BaseBuffer::~BaseBuffer() {
  vkDeviceWaitIdle(logicalDevice_->Get());
  vmaDestroyBuffer(allocator_->Get(), buffer_, allocation_);
}

void BaseBuffer::CreateBuffer(const VkDeviceSize size,
                              const VkBufferUsageFlags usage,
                              const VmaAllocationCreateFlags flags) {
  const ServiceLocator* sl = ServiceLocator::GetInstance();
  allocator_ = sl->getService<Memory::IAllocator>();
  logicalDevice_ = sl->getService<Devices::ILogicalDevice>();

  assert(size != 0);
  assert(usage != 0);

  VkBufferCreateInfo bufferInfo = {};
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = size;
  bufferInfo.usage = usage;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  VmaAllocationCreateInfo allocInfo = {};
  allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
  allocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
                    VMA_ALLOCATION_CREATE_MAPPED_BIT;

  VK_CHECK(vmaCreateBuffer(allocator_->Get(), &bufferInfo, &allocInfo, &buffer_,
                           &allocation_, &allocationInfo_));
}