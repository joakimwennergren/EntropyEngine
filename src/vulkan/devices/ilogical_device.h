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

#ifndef ENTROPY_VULKAN_ILOGICAL_DEVICE_H
#define ENTROPY_VULKAN_ILOGICAL_DEVICE_H

#include <vulkan/vulkan.hpp>
#include "servicelocators/servicelocator.h"

namespace Entropy::Graphics::Vulkan::Devices {
class ILogicalDevice : public IService {
public:
  ~ILogicalDevice() override = default;
  virtual VkDevice Get() = 0;
  virtual VkQueue GetGraphicQueue() = 0;

  // QueueFamily index
  uint32_t queueFamiliyIndex = 0;
};
} // namespace Entropy::Graphics::Vulkan::Devices

#endif // ENTROPY_VULKAN_ILOGICAL_DEVICE_H