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

#ifndef ENTROPY_VULKAN_SWAPCHAIN_H
#define ENTROPY_VULKAN_SWAPCHAIN_H

#include <vulkan/vulkan.hpp>
#include "iswapchain.h"
#include "vulkan/devices/ilogical_device.h"
#include "vulkan/devices/iphysical_device.h"

namespace Entropy::Graphics::Vulkan::SwapChains {

struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities{};
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> presentModes;
};

/**
 * @class SwapChain
 * @brief Manages the sequence of buffers that are displayed on the screen,
 * enabling double or triple buffering.
 *
 * A SwapChain is responsible for preparing and presenting rendered images to a
 * display. It handles the back buffer, where rendering occurs, and the front
 * buffer, which is displayed on the screen. This mechanism helps improve
 * performance and minimize visual artifacts.
 *
 * Responsibilities of the SwapChain include:
 * - Managing frame synchronization between rendering and presentation.
 * - Providing methods for swapping between the front and back buffers.
 * - Managing the lifecycle and resources associated with the swapping process.
 *
 * Typically used in graphical applications and game engines to manage rendering
 * and display efficiently.
 */
class SwapChain final : public ServiceBase<ISwapChain> {
public:
  SwapChain();
  ~SwapChain() override;

  void Build(std::shared_ptr<Surfaces::Surface> surface, VkExtent2D frame,
             VkSwapchainKHR oldSwapChain) override;

  void RecreateSwapChain(uint32_t width, uint32_t height) override;

  VkSwapchainKHR Get() override { return swapChain_; };

private:
  static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(
      const std::vector<VkSurfaceFormatKHR> &availableFormats);

  static VkPresentModeKHR ChooseSwapPresentMode(
      const std::vector<VkPresentModeKHR> &availablePresentModes);

  static VkExtent2D
  ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities,
                   VkExtent2D frame);

  static SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device,
                                                       VkSurfaceKHR surface);
  VkSwapchainKHR swapChain_ = VK_NULL_HANDLE;
  std::shared_ptr<Devices::IPhysicalDevice> physicalDevice_;
  std::shared_ptr<Devices::ILogicalDevice> logicalDevice_;
  std::shared_ptr<Surfaces::Surface> surface_;
};
} // namespace Entropy::Graphics::Vulkan::SwapChains

#endif // ENTROPY_VULKAN_SWAPCHAIN_H
