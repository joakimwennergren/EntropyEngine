#include "descriptorset_layout.h"
#include "vulkan/utilities/helpers.h"

using namespace Entropy::Graphics::Vulkan::DescriptorSetLayouts;
using namespace Entropy::Graphics::Vulkan::Devices;

DescriptorSetLayout::DescriptorSetLayout(
    const std::vector<VkDescriptorSetLayoutBinding> &layoutBindings,
    const std::vector<VkDescriptorBindingFlags> &bindingFlags) {

  const ServiceLocator *sl = ServiceLocator::GetInstance();
  logicalDevice_ = sl->getService<ILogicalDevice>();

  VkDescriptorSetLayoutBindingFlagsCreateInfo bindingFlagsInfo = {};
  bindingFlagsInfo.sType =
      VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO;
  bindingFlagsInfo.bindingCount = static_cast<uint32_t>(bindingFlags.size());
  bindingFlagsInfo.pBindingFlags = bindingFlags.data();

  VkDescriptorSetLayoutCreateInfo layoutInfo{};
  layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  layoutInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());
  layoutInfo.pBindings = layoutBindings.data();
  layoutInfo.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT;
  layoutInfo.pNext = &bindingFlagsInfo;

  VK_CHECK(vkCreateDescriptorSetLayout(logicalDevice_->Get(), &layoutInfo,
                                       nullptr, &descriptorSetLayout_));
}

DescriptorSetLayout::~DescriptorSetLayout() {
  vkDestroyDescriptorSetLayout(logicalDevice_->Get(), descriptorSetLayout_,
                               nullptr);
}
