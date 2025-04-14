#include "swapchain_texture.h"
#include <vulkan/vulkan.hpp>
#include "config.h"
#include "vulkan/utilities/helpers.h"

using namespace Entropy::Vulkan::Textures;

SwapChainTexture::SwapChainTexture(const uint32_t width,
                                   const uint32_t height) {

  VkImageCreateInfo imageInfo{};
  imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  imageInfo.imageType = VK_IMAGE_TYPE_2D;
  imageInfo.extent.width = width;
  imageInfo.extent.height = height;
  imageInfo.extent.depth = 1;
  imageInfo.mipLevels = 1;
  imageInfo.arrayLayers = 1;
  imageInfo.format = COLOR_FORMAT;
  imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
  imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  imageInfo.usage =
      VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
  imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
  imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  VmaAllocationCreateInfo allocCreateInfo = {};
  allocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
  allocCreateInfo.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;

  VK_CHECK(vmaCreateImage(allocator_->Get(), &imageInfo, &allocCreateInfo,
                          &textureImage_, &allocation_, nullptr));

  imageView_ = std::make_shared<ImageView>(textureImage_, COLOR_FORMAT);
};