#ifndef ENTROPY_VULKAN_SYNCHRONIZER_H
#define ENTROPY_VULKAN_SYNCHRONIZER_H

#include <vulkan/vulkan.hpp>
#include "vulkan/devices/ilogical_device.h"

namespace Entropy::Vulkan::Synchronization {

class Synchronizer {
 public:
  explicit Synchronizer(uint32_t numObjects);
  ~Synchronizer();

  std::vector<VkSemaphore> GetImageSemaphores() { return imageSemaphores_; };

  std::vector<VkSemaphore> GetRenderFinishedSemaphores() {
    return renderFinishedSemaphores_;
  };

  std::vector<VkFence> GetFences() { return fences_; };

 private:
  std::vector<VkSemaphore> imageSemaphores_;
  std::vector<VkSemaphore> renderFinishedSemaphores_;
  std::vector<VkFence> fences_;
  std::shared_ptr<Devices::ILogicalDevice> logicalDevice_;
  unsigned int numObjects_{};
};
}  // namespace Entropy::Vulkan::Synchronization

#endif  // ENTROPY_VULKAN_SYNCHRONIZER_H
