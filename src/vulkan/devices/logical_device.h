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

#ifndef ENTROPY_VULKAN_LOGICAL_DEVICE_H
#define ENTROPY_VULKAN_LOGICAL_DEVICE_H

#include <vulkan/vulkan.hpp>

#include "config.h"
#include "ilogical_device.h"
#include "iphysical_device.h"

namespace Entropy::Graphics::Vulkan::Devices {

class LogicalDevice final : public ServiceBase<ILogicalDevice> {
public:
  LogicalDevice();
  ~LogicalDevice() override;
  VkDevice Get() override { return logicalDevice_; }
  VkQueue GetGraphicQueue() override { return graphicsQueue_; }

private:
  VkDevice logicalDevice_ = VK_NULL_HANDLE;
  VkQueue graphicsQueue_ = VK_NULL_HANDLE;


#if PLATFORM == MACOS
  std::vector<const char *> deviceExtensions_ = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    VK_KHR_SHADER_DRAW_PARAMETERS_EXTENSION_NAME,
    "VK_KHR_portability_subset"};
#endif

#if PLATFORM == IOS
  std::vector<const char *> deviceExtensions_ = {VK_KHR_SWAPCHAIN_EXTENSION_NAME,
                                                "VK_KHR_portability_subset",
                                                "VK_EXT_metal_objects"};
#endif

#if PLATFORM == WINDOWS
  std::vector<const char *> deviceExtensions_ = {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME};
#endif

#if PLATFORM == ANDROID
  std::vector<const char *> deviceExtensions_ = {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME};
#endif

};

} // namespace Entropy::Graphics::Vulkan::Devices

#endif // ENTROPY_VULKAN_LOGICAL_DEVICE_H
