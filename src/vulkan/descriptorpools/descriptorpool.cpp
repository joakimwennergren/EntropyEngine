#include "descriptorpool.h"
#include "config.h"
#include "vulkan/utilities/helpers.h"

using namespace Entropy::Graphics::Vulkan::DescriptorPools;
using namespace Entropy::Graphics::Vulkan::Devices;

DescriptorPool::DescriptorPool() {
  const ServiceLocator *sl = ServiceLocator::GetInstance();
  logicalDevice_ = sl->getService<ILogicalDevice>();

  std::array<VkDescriptorPoolSize, 2> poolSizes{};

  poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  poolSizes[0].descriptorCount = 1;

  poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  poolSizes[1].descriptorCount = 1;

  VkDescriptorPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
  poolInfo.pPoolSizes = poolSizes.data();
  poolInfo.maxSets = 1;
  poolInfo.flags = 0;

  VK_CHECK(vkCreateDescriptorPool(logicalDevice_->Get(), &poolInfo, nullptr,
                                  &descriptorPool_));
}

DescriptorPool::~DescriptorPool() {
  vkDestroyDescriptorPool(logicalDevice_->Get(), descriptorPool_, nullptr);
}
