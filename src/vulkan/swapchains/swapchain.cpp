#include "swapchain.h"

#include <iostream>

#include "config.h"
#include "vulkan/imageviews/imageview.h"
#include "vulkan/queuefamilies/queuefamily.h"
#include "vulkan/utilities/helpers.h"

using namespace Entropy::Vulkan::SwapChains;
using namespace Entropy::Vulkan::ImageViews;
using namespace Entropy::Vulkan::QueueFamilies;
using namespace Entropy::Vulkan::Devices;

SwapChain::SwapChain() {
  const ServiceLocator* sl = ServiceLocator::GetInstance();
  physicalDevice_ = sl->GetService<IPhysicalDevice>();
  logicalDevice_ = sl->GetService<ILogicalDevice>();
}

SwapChain::~SwapChain() {
  vkDestroySwapchainKHR(logicalDevice_->Get(), swapChain_, nullptr);
}

void SwapChain::Build(const std::shared_ptr<Surfaces::Surface> surface,
                      const VkExtent2D frame,
                      const VkSwapchainKHR oldSwapChain) {
  surface_ = surface;
  auto [capabilities, formats, presentModes] =
      QuerySwapChainSupport(physicalDevice_->Get(), surface->Get());

  auto [format, colorSpace] = ChooseSwapSurfaceFormat(formats);

  auto [graphicsFamily, presentFamily] =
      QueueFamily::FindQueueFamilies(physicalDevice_->Get(), surface->Get());

  const uint32_t queueFamilyIndices[] = {graphicsFamily.value(),
                                         presentFamily.value()};

  const VkPresentModeKHR presentMode = ChooseSwapPresentMode(presentModes);

  const VkExtent2D extent = ChooseSwapExtent(capabilities, frame);

  uint32_t imageCount = capabilities.minImageCount + 1;
  if (capabilities.maxImageCount > 0 &&
      imageCount > capabilities.maxImageCount) {
    imageCount = capabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = surface->Get();
  createInfo.minImageCount = imageCount;
  createInfo.imageFormat = format;
  createInfo.imageColorSpace = colorSpace;
  createInfo.imageExtent = extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  if (graphicsFamily != presentFamily) {
    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices = queueFamilyIndices;
  } else {
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 0;
    createInfo.pQueueFamilyIndices = nullptr;
  }
  createInfo.preTransform = capabilities.currentTransform;

#if ENTROPY_PLATFORM == WINDOWS || ENTROPY_PLATFORM == LINUX
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
#else
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;
#endif
  createInfo.presentMode = presentMode;
  createInfo.clipped = VK_TRUE;
  createInfo.oldSwapchain = oldSwapChain;

  VK_CHECK(vkCreateSwapchainKHR(logicalDevice_->Get(), &createInfo, nullptr,
                                &swapChain_));

  vkDestroySwapchainKHR(logicalDevice_->Get(), oldSwapChain, nullptr);

  swapChainImageFormat = format;
  swapChainExtent = extent;

  imageCount = capabilities.minImageCount + 1;

  VK_CHECK(vkGetSwapchainImagesKHR(logicalDevice_->Get(), swapChain_,
                                   &imageCount, nullptr));

  swapChainImages.resize(imageCount);

  VK_CHECK(vkGetSwapchainImagesKHR(logicalDevice_->Get(), swapChain_,
                                   &imageCount, swapChainImages.data()));

  assert(!swapChainImages.empty());

  swapChainImageViews.resize(swapChainImages.size());

  for (uint32_t i = 0; i < swapChainImages.size(); i++) {
    swapChainImageViews[i] =
        std::make_shared<ImageView>(swapChainImages[i], swapChainImageFormat);
  }
}

void SwapChain::RecreateSwapChain(const uint32_t width, const uint32_t height) {
  swapChainImageFormat = COLOR_FORMAT;
  swapChainExtent = VkExtent2D{width, height};
  swapChainImageViews.clear();
  swapChainImages.clear();
  Build(surface_, swapChainExtent, swapChain_);
}

VkSurfaceFormatKHR SwapChain::ChooseSwapSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR>& availableFormats) {
  for (const auto& availableFormat : availableFormats) {
    if (availableFormat.format == COLOR_FORMAT &&
        availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return availableFormat;
    }
  }
  return availableFormats[0];
}

VkPresentModeKHR SwapChain::ChooseSwapPresentMode(
    const std::vector<VkPresentModeKHR>& availablePresentModes) {
  for (const auto& availablePresentMode : availablePresentModes) {
    if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
      return availablePresentMode;
    }
  }

  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SwapChain::ChooseSwapExtent(
    const VkSurfaceCapabilitiesKHR& capabilities, const VkExtent2D frame) {
  if (capabilities.currentExtent.width !=
      std::numeric_limits<uint32_t>::max()) {
    return capabilities.currentExtent;
  }
  VkExtent2D actualExtent = {(frame.width), (frame.height)};
  actualExtent.width =
      std::clamp(actualExtent.width, capabilities.minImageExtent.width,
                 capabilities.maxImageExtent.width);
  actualExtent.height =
      std::clamp(actualExtent.height, capabilities.minImageExtent.height,
                 capabilities.maxImageExtent.height);
  return actualExtent;
}

SwapChainSupportDetails SwapChain::QuerySwapChainSupport(
    VkPhysicalDevice device, VkSurfaceKHR surface) {
  SwapChainSupportDetails details;

  VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface,
                                                     &details.capabilities));

  uint32_t formatCount;
  VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount,
                                                nullptr));

  if (formatCount != 0) {
    details.formats.resize(formatCount);
    VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount,
                                                  details.formats.data()));
  }

  uint32_t presentModeCount;
  VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(
      device, surface, &presentModeCount, nullptr));

  if (presentModeCount != 0) {
    details.presentModes.resize(presentModeCount);
    VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(
        device, surface, &presentModeCount, details.presentModes.data()));
  }

  return details;
}
