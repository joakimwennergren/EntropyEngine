#include "logical_device.h"
#include "vulkan/utilities/helpers.h"

using namespace Entropy::Graphics::Vulkan::Devices;

LogicalDevice::LogicalDevice() {
  const ServiceLocator *sl = ServiceLocator::GetInstance();
  const auto physicalDevice = sl->getService<IPhysicalDevice>();

  uint32_t idx = 0;
  uint32_t queueFamilyPropertyCount;
  std::vector<VkQueueFamilyProperties> queueFamilyProperties;

  vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice->Get(),
                                           &queueFamilyPropertyCount, nullptr);

  assert(queueFamilyPropertyCount != 0);
  queueFamilyProperties.resize(queueFamilyPropertyCount);

  vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice->Get(),
                                           &queueFamilyPropertyCount,
                                           queueFamilyProperties.data());

  for (uint32_t i = 0; i < queueFamilyPropertyCount; i++) {
    if (queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      queueFamiliyIndex = idx;
      break;
    }
    ++idx;
  }

  float queuePriority = 1.0f;

  VkDeviceQueueCreateInfo queueCreateInfo{};
  queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queueCreateInfo.queueFamilyIndex = queueFamiliyIndex;
  queueCreateInfo.queueCount = 1;
  queueCreateInfo.pQueuePriorities = &queuePriority;

  VkPhysicalDeviceFeatures deviceFeatures{};
  deviceFeatures.samplerAnisotropy = VK_TRUE;
  deviceFeatures.fillModeNonSolid = VK_TRUE;

  VkPhysicalDeviceDescriptorIndexingFeatures indexingFeatures = {};
  indexingFeatures.sType =
      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES;
  indexingFeatures.descriptorBindingPartiallyBound = VK_TRUE;

  VkDeviceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  createInfo.queueCreateInfoCount = 1;
  createInfo.pQueueCreateInfos = &queueCreateInfo;
  createInfo.pEnabledFeatures = &deviceFeatures;
  createInfo.enabledExtensionCount =
      static_cast<uint32_t>(deviceExtensions_.size());
  createInfo.ppEnabledExtensionNames = deviceExtensions_.data();
  createInfo.pNext = &indexingFeatures;

  VK_CHECK(vkCreateDevice(physicalDevice->Get(), &createInfo, nullptr,
                          &logicalDevice_));

  vkGetDeviceQueue(logicalDevice_, queueFamiliyIndex, 0, &graphicsQueue_);
}

LogicalDevice::~LogicalDevice() { vkDestroyDevice(logicalDevice_, nullptr); }

/*

LogicalDevice::LogicalDevice(std::shared_ptr<PhysicalDevice> physicalDevice,
                             std::shared_ptr<WindowSurface> surface) {
  QueueFamilyIndices indices =
      QueueFamily::FindQueueFamilies(physicalDevice->Get(), surface->Get());

  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
  std::vector<uint32_t> uniqueQueueFamilies = {
      //indices.graphicsFamily.value(),
indices.presentFamily.value()
}
;

float queuePriority = 1.0f;
for (uint32_t queueFamily : uniqueQueueFamilies) {
  VkDeviceQueueCreateInfo queueCreateInfo{};
  queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queueCreateInfo.queueFamilyIndex = queueFamily;
  queueCreateInfo.queueCount = 1;
  queueCreateInfo.pQueuePriorities = &queuePriority;
  queueCreateInfos.push_back(queueCreateInfo);
}

VkPhysicalDeviceFeatures deviceFeatures{};
deviceFeatures.samplerAnisotropy = VK_TRUE;
deviceFeatures.fillModeNonSolid = VK_TRUE;

VkDeviceCreateInfo createInfo{};
createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
createInfo.queueCreateInfoCount =
    static_cast<uint32_t>(queueCreateInfos.size());
createInfo.pQueueCreateInfos = queueCreateInfos.data();
createInfo.pEnabledFeatures = &deviceFeatures;
createInfo.enabledExtensionCount =
    static_cast<uint32_t>(physicalDevice->deviceExtensions.size());
createInfo.ppEnabledExtensionNames = physicalDevice->deviceExtensions.data();

if (vkCreateDevice(physicalDevice->Get(), &createInfo, nullptr,
                   &_logicalDevice) != VK_SUCCESS) {
  exit(EXIT_FAILURE);
}

// vkGetDeviceQueue(_logicalDevice, indices.graphicsFamily.value(), 0,
// &_graphicsQueue);
vkGetDeviceQueue(_logicalDevice, indices.presentFamily.value(), 0,
                 &_presentQueue);
}


*/