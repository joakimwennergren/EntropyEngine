#include "commandbuffer.h"
#include "vulkan/utilities/helpers.h"

using namespace Entropy::Graphics::Vulkan::CommandBuffers;
using namespace Entropy::Graphics::Vulkan::CommandPools;
using namespace Entropy::Graphics::Vulkan::Devices;

CommandBuffer::CommandBuffer(const VkCommandBufferLevel level) {
  const ServiceLocator *sl = ServiceLocator::GetInstance();
  logicalDevice_ = sl->getService<ILogicalDevice>();
  commandPool_ = sl->getService<ICommandPool>();

  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool = commandPool_->Get();
  allocInfo.level = level;
  allocInfo.commandBufferCount = 1;

  VK_CHECK(vkAllocateCommandBuffers(logicalDevice_->Get(), &allocInfo,
                                    &commandBuffer_));
}

CommandBuffer::~CommandBuffer() {
  vkFreeCommandBuffers(logicalDevice_->Get(), commandPool_->Get(), 1,
                       &commandBuffer_);
}

void CommandBuffer::RecordOnce() const {
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

  VK_CHECK(vkBeginCommandBuffer(commandBuffer_, &beginInfo));
}

void CommandBuffer::Record() const {
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = 0;
  beginInfo.pInheritanceInfo = nullptr;

  VK_CHECK(vkBeginCommandBuffer(commandBuffer_, &beginInfo));
}

void CommandBuffer::EndRecording() const {
  VK_CHECK(vkEndCommandBuffer(commandBuffer_));
}

void CommandBuffer::EndRecordingOnce() const {
  VK_CHECK(vkEndCommandBuffer(commandBuffer_));
}
