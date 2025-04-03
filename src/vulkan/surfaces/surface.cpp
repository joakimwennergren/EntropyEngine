#include "surface.h"

#include <iostream>
#include <vulkan/devices/ilogical_device.h>

#include "servicelocators/servicelocator.h"
#include "vulkan/utilities/helpers.h"
#include "vulkan/instances/vk_instance.h"

using namespace Entropy::Graphics::Vulkan::Surfaces;
using namespace Entropy::Graphics::Vulkan::Instances;

#if PLATFORM == MACOS || PLATFORM == LINUX

Surface::Surface(GLFWwindow *window) {
  const ServiceLocator *sl = ServiceLocator::GetInstance();
  instance_ = sl->getService<IVulkanInstance>();
  VK_CHECK(
      glfwCreateWindowSurface(instance_->Get(), window, nullptr, &surface_));
}

#endif

/*
Surface::Surface(const VkSurfaceKHR surface) {
    const ServiceLocator *sl = ServiceLocator::GetInstance();
    instance_ = sl->getService<IVulkanInstance>();
    surface_ = surface;
}
*/

#if PLATFORM == WINDOWS
Surface::Surface(GLFWwindow *window) {


  const ServiceLocator *sl = ServiceLocator::GetInstance();
  instance_ = sl->getService<IVulkanInstance>();


  VkWin32SurfaceCreateInfoKHR createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
  createInfo.hwnd = glfwGetWin32Window(window);
  createInfo.hinstance = GetModuleHandle(nullptr);


  VK_CHECK(vkCreateWin32SurfaceKHR(instance_->Get(), &createInfo, nullptr,
                              &this->surface_));
}
#endif

#if PLATFORM == IOS
Surface::Surface(CA::MetalLayer *layer) {

  const ServiceLocator *sl = ServiceLocator::GetInstance();
  instance_ = sl->getService<IVulkanInstance>();

  VkMetalSurfaceCreateInfoEXT createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_METAL_SURFACE_CREATE_INFO_EXT;
  createInfo.pNext = nullptr;
  createInfo.flags = 0;
  createInfo.pLayer= layer;



  VK_CHECK(vkCreateMetalSurfaceEXT(instance_->Get(), &createInfo, nullptr, &surface_));
}
#endif

Surface::~Surface() {
    vkDestroySurfaceKHR(instance_->Get(), surface_, nullptr);
}