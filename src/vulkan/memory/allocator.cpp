#define VMA_IMPLEMENTATION

#include "allocator.h"
#include "vulkan/devices/ilogical_device.h"
#include "vulkan/devices/iphysical_device.h"
#include "vulkan/instances/ivk_instance.h"
#include "vulkan/utilities/helpers.h"

using namespace Entropy::Vulkan::Memory;
using namespace Entropy::Vulkan::Instances;
using namespace Entropy::Vulkan::Devices;

Allocator::Allocator() {
  const ServiceLocator* sl = ServiceLocator::GetInstance();
  const auto vulkanInstance = sl->GetService<IVulkanInstance>();
  const auto physicalDevice = sl->GetService<IPhysicalDevice>();
  const auto logicalDevice = sl->GetService<ILogicalDevice>();

  VmaVulkanFunctions vulkanFunctions = {};
  vulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
  vulkanFunctions.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;

  VmaAllocatorCreateInfo allocatorCreateInfo = {};
  allocatorCreateInfo.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
  allocatorCreateInfo.vulkanApiVersion =
      VK_API_VERSION_1_3;  // @TODO make this configurable?
  allocatorCreateInfo.physicalDevice = physicalDevice->Get();
  allocatorCreateInfo.device = logicalDevice->Get();
  allocatorCreateInfo.instance = vulkanInstance->Get();
  allocatorCreateInfo.pVulkanFunctions = &vulkanFunctions;

  VK_CHECK(vmaCreateAllocator(&allocatorCreateInfo, &_allocator));
}

Allocator::~Allocator() {
  vmaDestroyAllocator(_allocator);
}