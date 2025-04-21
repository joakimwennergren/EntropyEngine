#include "commandpool.h"
#include "vulkan/utilities/helpers.h"

using namespace Entropy::Vulkan::CommandPools;
using namespace Entropy::Vulkan::Devices;

CommandPool::CommandPool() {
  const ServiceLocator* sl = ServiceLocator::GetInstance();
  logicalDevice_ = sl->GetService<ILogicalDevice>();

  VkCommandPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  poolInfo.queueFamilyIndex = logicalDevice_->queueFamiliyIndex;

  VK_CHECK(vkCreateCommandPool(logicalDevice_->Get(), &poolInfo, nullptr,
                               &commandPool_));
}

CommandPool::~CommandPool() {
  vkDestroyCommandPool(logicalDevice_->Get(), commandPool_, nullptr);
}