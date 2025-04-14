#include "physical_device.h"

#include "vulkan/instances/ivk_instance.h"
#include "vulkan/utilities/helpers.h"

using namespace Entropy::Vulkan::Devices;
using namespace Entropy::Vulkan::Instances;

PhysicalDevice::PhysicalDevice() {
  const ServiceLocator* sl = ServiceLocator::GetInstance();
  const auto vulkanInstance = sl->getService<IVulkanInstance>();

  uint32_t deviceCount;
  std::vector<VkPhysicalDevice> physicalDevices;

  VK_CHECK(
      vkEnumeratePhysicalDevices(vulkanInstance->Get(), &deviceCount, nullptr));

  assert(deviceCount != 0);
  physicalDevices.resize(deviceCount);

  VK_CHECK(vkEnumeratePhysicalDevices(vulkanInstance->Get(), &deviceCount,
                                      physicalDevices.data()));

  _physicalDevice = physicalDevices[0];

  // @TODO make sure to choose the most suitable physical device!
  /*
  for (const auto &device : physicalDevices) {
    if (IsDeviceSuitable(device, surface->Get(), deviceExtensions)) {
      _physicalDevice = device;
      break;
    }
  }
  */
}

/*
bool PhysicalDevice::IsDeviceSuitable(
    VkPhysicalDevice device, VkSurfaceKHR surface,
    const std::vector<const char *> deviceExtensions) {
  QueueFamilyIndices indices = QueueFamily::FindQueueFamilies(device, surface);

  bool extensionsSupported =
      CheckDeviceExtensionSupport(device, deviceExtensions);

  bool swapChainAdequate = false;
  if (extensionsSupported) {
    SwapChainSupportDetails swapChainSupport =
        Swapchain::QuerySwapChainSupport(device, surface);
    swapChainAdequate = !swapChainSupport.formats.empty() &&
                        !swapChainSupport.presentModes.empty();
  }

  return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

bool PhysicalDevice::CheckDeviceExtensionSupport(
    VkPhysicalDevice device, const std::vector<const char *> extensions) {
  uint32_t extensionCount;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       nullptr);

  std::vector<VkExtensionProperties> availableExtensions(extensionCount);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       availableExtensions.data());

  std::set<std::string> requiredExtensions(extensions.begin(),
                                           extensions.end());

  for (const auto &extension : availableExtensions) {
    requiredExtensions.erase(extension.extensionName);
  }

  return requiredExtensions.empty();
}
*/