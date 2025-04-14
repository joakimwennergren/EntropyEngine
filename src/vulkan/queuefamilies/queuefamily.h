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

#ifndef ENTROPY_VULKAN_QUEUEFAMILY_H
#define ENTROPY_VULKAN_QUEUEFAMILY_H

#include <optional>
#include <vulkan/vulkan.hpp>

namespace Entropy::Vulkan::QueueFamilies {

struct QueueFamilyIndices {
  std::optional<uint32_t> graphicsFamily;
  std::optional<uint32_t> presentFamily;
  [[nodiscard]] bool isComplete() const {
    return graphicsFamily.has_value() && presentFamily.has_value();
  }
};

class QueueFamily {
 public:
  static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device,
                                              VkSurfaceKHR surface);
};
}  // namespace Entropy::Vulkan::QueueFamilies

#endif  // ENTROPY_VULKAN_QUEUEFAMILY_H
