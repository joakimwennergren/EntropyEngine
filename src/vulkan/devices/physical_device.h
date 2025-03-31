// Copyright 2025 Joakim Wennergren
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef ENTROPY_VULKAN_PHYSICAL_DEVICE_H
#define ENTROPY_VULKAN_PHYSICAL_DEVICE_H

#include <vulkan/vulkan.hpp>
#include "vulkan/devices/iphysical_device.h"

namespace Entropy::Graphics::Vulkan::Devices {

/**
 * @brief A service class that wraps and manages a Vulkan physical device.
 *
 * The PhysicalDevice class provides functionality for managing a Vulkan
 * physical device, including selecting a suitable device and verifying its
 * capabilities. It acts as an implementation for the IPhysicalDevice interface.
 *
 * This class is responsible for encapsulating the Vulkan physical device and
 * provides methods for querying and ensuring physical device requirements are
 * met, such as checking extension support and device suitability.
 */
class PhysicalDevice final : public ServiceBase<IPhysicalDevice> {
public:
  PhysicalDevice();
  VkPhysicalDevice Get() override { return _physicalDevice; }

private:
  /**
   * @brief Checks if a given Vulkan physical device is suitable for rendering.
   *
   * This method evaluates if the specified Vulkan physical device meets the
   * necessary criteria for rendering by checking for required device extensions
   * and surface capabilities.
   *
   * @param device The Vulkan physical device to be evaluated.
   * @param surface The Vulkan surface for presentation support.
   * @param deviceExtensions List of required device extensions.
   *
   * @return Returns true if the device is suitable for rendering, otherwise
   * returns false.
   */
  bool IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface,
                        std::vector<const char *> deviceExtensions);

  /**
   * @brief Checks if the specified Vulkan physical device supports the required
   * extensions.
   *
   * This method iterates through the list of required extensions and verifies
   * if all of them are supported by the Vulkan physical device provided.
   *
   * @param device The Vulkan physical device to be checked.
   * @param extensions A list of required extension names to check for support.
   *
   * @return Returns true if all specified extensions are supported by the
   * device; otherwise, returns false.
   */
  bool CheckDeviceExtensionSupport(VkPhysicalDevice device,
                                   std::vector<const char *> extensions);

  // physical device handle
  VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
};
} // namespace Entropy::Graphics::Vulkan::Devices

#endif // ENTROPY_VULKAN_PHYSICAL_DEVICE_H
