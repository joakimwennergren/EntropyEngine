#include "vk_instance.h"
#include "vulkan/utilities/helpers.h"
#include "config.h"

using namespace Entropy::Graphics::Vulkan::Instances;
using namespace Entropy::Graphics::Vulkan::ValidationLayers;

VulkanInstance::VulkanInstance() {
  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Entropy Application";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "Entropy";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_3;

  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

#if PLATFORM == MACOS
  createInfo.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

  uint32_t extensionCount = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

  std::vector<VkExtensionProperties> extensionProps(extensionCount);
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount,
                                         extensionProps.data());

  std::vector<const char *> extensions;
  for (const auto &extension : extensionProps) {
    extensions.push_back(extension.extensionName);
  }
  extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);

#if PLATFORM == MACOS
  extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
  extensions.push_back("VK_MVK_macos_surface");
  createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  createInfo.ppEnabledExtensionNames = extensions.data();
#endif

#if PLATFORM ==LINUX
  createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  createInfo.ppEnabledExtensionNames = extensions.data();
#endif

#if PLATFORM == ANDROID
  extensions.push_back("VK_KHR_surface");
  extensions.push_back("VK_KHR_android_surface");
  // extensions.push_back("VK_KHR_surface_protected_capabilities");
  createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  createInfo.ppEnabledExtensionNames = extensions.data();
#endif

#if PLATFORM == WINDOWS
  createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  createInfo.ppEnabledExtensionNames = extensions.data();
#endif

#if USE_VALIDATION_LAYERS == 1
  createInfo.enabledLayerCount =
      static_cast<uint32_t>(validationLayer_.layers.size());
  createInfo.ppEnabledLayerNames = validationLayer_.layers.data();
#endif

  VK_CHECK(vkCreateInstance(&createInfo, nullptr, &instance_));

#if USE_VALIDATION_LAYERS == 1
  VkDebugUtilsMessengerCreateInfoEXT dbgCreateInfo{};
  dbgCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  dbgCreateInfo.messageSeverity =
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  dbgCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                              VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                              VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  dbgCreateInfo.pfnUserCallback = ValidationLayer::debugCallback;
  dbgCreateInfo.pUserData = nullptr;

  VK_CHECK(ValidationLayer::CreateDebugUtilsMessengerEXT(
      instance_, &dbgCreateInfo, nullptr, &validationLayer_.debugMessenger));
  #endif
}

VulkanInstance::~VulkanInstance() {
  ValidationLayer::DestroyDebugUtilsMessengerEXT(
      instance_, validationLayer_.debugMessenger, nullptr);
  vkDestroyInstance(instance_, nullptr);
}
