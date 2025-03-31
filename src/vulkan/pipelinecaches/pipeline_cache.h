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

#ifndef ENTROPY_VULKAN_PIPELINE_CACHE_H
#define ENTROPY_VULKAN_PIPELINE_CACHE_H

#include "ipipeline_cache.h"
#include "vulkan/devices/ilogical_device.h"

namespace Entropy::Graphics::Vulkan::Caches {
/**
 * @class PipelineCache
 * @brief A final class that implements the IPipelineCache interface to manage
 * Vulkan pipeline cache operations.
 *
 * The PipelineCache class is responsible for handling Vulkan pipeline caching
 * through encapsulating the VkPipelineCache object. It leverages logical
 * devices to interact with the Vulkan API and provides access to the Vulkan
 * pipeline cache.
 *
 * This class derives from ServiceBase with IPipelineCache as its interface
 * type.
 */
class PipelineCache final : public ServiceBase<IPipelineCache> {
public:
  PipelineCache();
  ~PipelineCache() override;
  VkPipelineCache Get() override { return pipelineCache_; }

private:
  VkPipelineCache pipelineCache_ = VK_NULL_HANDLE;
  std::shared_ptr<Devices::ILogicalDevice> logicalDevice_;
};
} // namespace Entropy::Graphics::Vulkan::Caches

#endif // ENTROPY_VULKAN_PIPELINE_CACHE_H
