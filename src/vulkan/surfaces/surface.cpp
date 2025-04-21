#include "surface.h"

#include <vulkan/devices/ilogical_device.h>
#include "servicelocators/servicelocator.h"
#include "vulkan/instances/vk_instance.h"
#include "vulkan/utilities/helpers.h"

using namespace Entropy::Vulkan::Surfaces;
using namespace Entropy::Vulkan::Instances;

#if ENTROPY_PLATFORM == MACOS || ENTROPY_PLATFORM == LINUX
Surface::Surface(GLFWwindow* window) {
  const ServiceLocator* sl = ServiceLocator::GetInstance();
  instance_ = sl->GetService<IVulkanInstance>();
  VK_CHECK(
      glfwCreateWindowSurface(instance_->Get(), window, nullptr, &surface_));
}
#elif ENTROPY_PLATFORM == IOS

Surface::Surface(CAMetalLayer* layer) {

  const ServiceLocator* sl = ServiceLocator::GetInstance();
  instance_ = sl->GetService<IVulkanInstance>();

  VkMetalSurfaceCreateInfoEXT createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_METAL_SURFACE_CREATE_INFO_EXT;
  createInfo.pNext = nullptr;
  createInfo.flags = 0;
  createInfo.pLayer = layer;

  VK_CHECK(vkCreateMetalSurfaceEXT(instance_->Get(), &createInfo, nullptr,
                                   &surface_));
}

#else
Surface::Surface(VkSurfaceKHR surface) {
  const ServiceLocator* sl = ServiceLocator::GetInstance();
  instance_ = sl->GetService<IVulkanInstance>();
  surface_ = surface;
}
#endif

/*
#if ENTROPY_PLATFORM == WINDOWS
Surface::Surface(GLFWwindow *window) {


  const ServiceLocator *sl = ServiceLocator::GetInstance();
  instance_ = sl->GetService<IVulkanInstance>();


  VkWin32SurfaceCreateInfoKHR createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
  createInfo.hwnd = glfwGetWin32Window(window);
  createInfo.hinstance = GetModuleHandle(nullptr);


  VK_CHECK(vkCreateWin32SurfaceKHR(instance_->Get(), &createInfo, nullptr,
                              &this->surface_));
}
#endif
*/

Surface::~Surface() {
  vkDestroySurfaceKHR(instance_->Get(), surface_, nullptr);
}