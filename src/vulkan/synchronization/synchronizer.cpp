#include "synchronizer.h"
#include "vulkan/utilities/helpers.h"

using namespace Entropy::Graphics::Vulkan::Synchronization;
using namespace Entropy::Graphics::Vulkan::Devices;

Synchronizer::Synchronizer(uint32_t numObjects) {
  assert(numObjects != 0);

  logicalDevice_ = ServiceLocator::GetInstance()->getService<ILogicalDevice>();
  numObjects_ = numObjects;

  imageSemaphores_.resize(numObjects);
  renderFinishedSemaphores_.resize(numObjects);
  fences_.resize(numObjects);

  VkSemaphoreCreateInfo semaphoreInfo{};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkFenceCreateInfo fenceInfo{};
  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  for (size_t i = 0; i < numObjects_; i++) {
    VK_CHECK(vkCreateSemaphore(logicalDevice_->Get(), &semaphoreInfo, nullptr,
      &imageSemaphores_[i]));
    VK_CHECK(vkCreateSemaphore(logicalDevice_->Get(), &semaphoreInfo, nullptr,
      &renderFinishedSemaphores_[i]));
    VK_CHECK(vkCreateFence(logicalDevice_->Get(), &fenceInfo, nullptr,
      &fences_[i]));
  }
}

Synchronizer::~Synchronizer() {
  // @todo remove this wait
  vkDeviceWaitIdle(logicalDevice_->Get());
  for (size_t i = 0; i < numObjects_; i++) {
    vkDestroySemaphore(logicalDevice_->Get(), imageSemaphores_[i], nullptr);
    vkDestroySemaphore(logicalDevice_->Get(), renderFinishedSemaphores_[i],
                       nullptr);
    vkDestroyFence(logicalDevice_->Get(), fences_[i], nullptr);
  }
}