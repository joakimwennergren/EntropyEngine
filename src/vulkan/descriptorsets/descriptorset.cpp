#include "descriptorset.h"
#include "vulkan/utilities/helpers.h"

using namespace Entropy::Graphics::Vulkan::DescriptorSets;
using namespace Entropy::Graphics::Vulkan::Devices;
using namespace Entropy::Graphics::Vulkan::DescriptorPools;
using namespace Entropy::Graphics::Vulkan::DescriptorSetLayouts;

DescriptorSet::DescriptorSet(const std::shared_ptr<DescriptorSetLayout> layout) {
  const ServiceLocator *sl = ServiceLocator::GetInstance();
  logicalDevice_ = sl->getService<ILogicalDevice>();
  descriptorPool_ = sl->getService<IDescriptorPool>();

  const std::array<VkDescriptorSetLayout, 1> layouts = {layout->Get()};

  VkDescriptorSetAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  allocInfo.descriptorPool = descriptorPool_->Get();
  allocInfo.descriptorSetCount = 1;
  allocInfo.pSetLayouts = layouts.data();

  VK_CHECK(vkAllocateDescriptorSets(logicalDevice_->Get(), &allocInfo,
    &descriptorSet_));
}

DescriptorSet::~DescriptorSet(){
  vkFreeDescriptorSets(logicalDevice_->Get(), descriptorPool_->Get(), 1, &descriptorSet_);
}