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

#ifndef ENTROPY_VULKAN_VKINSTANCE_H
#define ENTROPY_VULKAN_VKINSTANCE_H

#include <vulkan/vulkan.hpp>

#include "ivk_instance.h"
#include "servicelocators/servicelocator.h"
#include "vulkan/validationlayers/validationlayer.h"

namespace Entropy::Graphics::Vulkan::Instances {

/**
 * @class VulkanInstance
 *
 * @brief Represents a Vulkan instance used for managing the Vulkan API state
 * and interfacing with the driver.
 *
 * The VulkanInstance class is responsible for creating and managing an instance
 * of the Vulkan API. It initializes the Vulkan runtime and provides a handle to
 * interact with the Vulkan platform.
 *
 * Key responsibilities of the VulkanInstance class include:
 * - Initializing the Vulkan API instance with required application info and
 * extensions.
 * - Managing extension and layer support.
 * - Cleaning up the Vulkan instance during destruction to avoid resource leaks.
 */
class VulkanInstance final : public ServiceBase<IVulkanInstance> {
public:
  VulkanInstance();
  ~VulkanInstance() override;
  VkInstance Get() override { return instance_; }

private:
  VkInstance instance_ = VK_NULL_HANDLE;
  ValidationLayers::ValidationLayer validationLayer_;
};
} // namespace Entropy::Graphics::Vulkan::Instances

#endif // ENTROPY_VULKAN_VKINSTANCE_H
