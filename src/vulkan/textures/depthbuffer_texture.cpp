#include "depthbuffer_texture.h"
#include "vulkan/utilities/helpers.h"

using namespace Entropy::Vulkan::Textures;

DepthBufferTexture::DepthBufferTexture(const uint32_t width,
                                       const uint32_t height) {

  VkFormat depthFormat =
      FindSupportedFormat({VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT,
                           VK_FORMAT_D24_UNORM_S8_UINT});

  VmaAllocationCreateInfo allocCreateInfo = {};
  allocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
  allocCreateInfo.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;

  VkImageCreateInfo imageInfo{};
  imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  imageInfo.imageType = VK_IMAGE_TYPE_2D;
  imageInfo.extent.width = width;
  imageInfo.extent.height = height;
  imageInfo.extent.depth = 1;
  imageInfo.mipLevels = 1;
  imageInfo.arrayLayers = 1;
  imageInfo.format = depthFormat;
  imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
  imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
  imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
  imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  VK_CHECK(vmaCreateImage(allocator_->Get(), &imageInfo, &allocCreateInfo,
                          &textureImage_, &allocation_, nullptr));

  imageView_ = std::make_shared<ImageView>(textureImage_, depthFormat,
                                           VK_IMAGE_ASPECT_DEPTH_BIT);
};

VkFormat DepthBufferTexture::FindSupportedFormat(
    const std::vector<VkFormat>& candidates) {
  for (const VkFormat format : candidates) {
    VkFormatProperties props;
    vkGetPhysicalDeviceFormatProperties(physicalDevice_->Get(), format, &props);
    if ((props.optimalTilingFeatures &
         VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) ==
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
      return format;
    }
  }

  LOG_INFO(logger_, "Failed to find supported depth format!");
  return VK_FORMAT_UNDEFINED;
}