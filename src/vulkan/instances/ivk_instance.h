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

#ifndef ENTROPY_VULKAN_IVKINSTANCE_H
#define ENTROPY_VULKAN_IVKINSTANCE_H

#include <vulkan/vulkan.hpp>
#include "servicelocators/servicelocator.h"

namespace Entropy::Vulkan::Instances {

/**
 * @class IVulkanInstance
 * @brief Interface class for managing a Vulkan instance.
 *
 * The IVulkanInstance class serves as an abstraction layer for handling the
 * creation and management of a Vulkan instance. It provides essential
 * functionality for initializing Vulkan, managing initialization parameters,
 * and cleaning up resources associated with the Vulkan instance.
 *
 * This interface typically includes methods for querying Vulkan support,
 * accessing instance extensions and layers, and handling the lifecycle of
 * the Vulkan instance.
 *
 * Functions in this interface must be implemented by a derived class to
 * ensure proper management of Vulkan initialization and teardown according
 * to the application's specific requirements.
 */
class IVulkanInstance : public IService {
 public:
  ~IVulkanInstance() override = default;
  virtual VkInstance Get() = 0;
};
}  // namespace Entropy::Vulkan::Instances

#endif  // ENTROPY_VULKAN_IVKINSTANCE_H
