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

#ifndef ENTROPY_VULKAN_IMAGEVIEW_H
#define ENTROPY_VULKAN_IMAGEVIEW_H

#include <vulkan/vulkan.hpp>
#include "vulkan/devices/ilogical_device.h"

namespace Entropy::Vulkan::ImageViews {

class ImageView {
 public:
  ImageView(VkImage image, VkFormat format,
            uint32_t flags = VK_IMAGE_ASPECT_COLOR_BIT,
            VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_2D,
            uint32_t layerCount = 1);
  ~ImageView();
  [[nodiscard]] VkImageView Get() const { return imageView_; }

 private:
  VkImageView imageView_ = VK_NULL_HANDLE;
  std::shared_ptr<Devices::ILogicalDevice> logicalDevice_;
};
}  // namespace Entropy::Vulkan::ImageViews

#endif  // ENTROPY_IMAGEVIEW_H
