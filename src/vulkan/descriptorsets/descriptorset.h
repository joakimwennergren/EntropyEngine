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

#ifndef ENTROPY_VULKAN_DESCRIPTORSET_H
#define ENTROPY_VULKAN_DESCRIPTORSET_H

#include <vulkan/vulkan.hpp>
#include "vulkan/descriptorpools/descriptorpool.h"
#include "vulkan/descriptorsetlayouts/descriptorset_layout.h"
#include "vulkan/devices/ilogical_device.h"

namespace Entropy::Vulkan::DescriptorSets {

class DescriptorSet {
 public:
  explicit DescriptorSet(
      std::shared_ptr<DescriptorSetLayouts::DescriptorSetLayout> layout);
  ~DescriptorSet();
  VkDescriptorSet Get() const { return descriptorSet_; }

 private:
  VkDescriptorSet descriptorSet_ = VK_NULL_HANDLE;
  std::shared_ptr<Devices::ILogicalDevice> logicalDevice_;
  std::shared_ptr<DescriptorPools::IDescriptorPool> descriptorPool_;
};
}  // namespace Entropy::Vulkan::DescriptorSets

#endif  // ENTROPY_VULKAN_DESCRIPTORSET_H
