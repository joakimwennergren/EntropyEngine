#include "imageview.h"
#include "vulkan/utilities/helpers.h"

using namespace Entropy::Graphics::Vulkan::ImageViews;
using namespace Entropy::Graphics::Vulkan::Devices;

ImageView::ImageView(VkImage image, const VkFormat format,
                     const uint32_t flags) {
  ServiceLocator *sl = ServiceLocator::GetInstance();
  logicalDevice_ = sl->getService<ILogicalDevice>();

  VkImageViewCreateInfo viewInfo{};
  viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  viewInfo.image = image;
  viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
  viewInfo.format = format;
  viewInfo.subresourceRange.aspectMask = flags;
  viewInfo.subresourceRange.baseMipLevel = 0;
  viewInfo.subresourceRange.levelCount = 1;
  viewInfo.subresourceRange.baseArrayLayer = 0;
  viewInfo.subresourceRange.layerCount = 1;

  VK_CHECK(vkCreateImageView(logicalDevice_->Get(), &viewInfo, nullptr,
                             &imageView_));
}

ImageView::~ImageView() {
  vkDestroyImageView(logicalDevice_->Get(), imageView_, nullptr);
}
