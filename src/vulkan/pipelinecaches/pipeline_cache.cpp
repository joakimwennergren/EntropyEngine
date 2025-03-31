#include "pipeline_cache.h"
#include "vulkan/utilities/helpers.h"

using namespace Entropy::Graphics::Vulkan::Caches;
using namespace Entropy::Graphics::Vulkan::Devices;

PipelineCache::PipelineCache() {
  const ServiceLocator *sl = ServiceLocator::GetInstance();
  logicalDevice_ = sl->getService<ILogicalDevice>();

  VkPipelineCacheCreateInfo pipelineCacheCreateInfo{};
  pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;

  VK_CHECK(vkCreatePipelineCache(logicalDevice_->Get(),
    &pipelineCacheCreateInfo, nullptr,
    &pipelineCache_));
}

PipelineCache::~PipelineCache() {
  vkDestroyPipelineCache(logicalDevice_->Get(), pipelineCache_, nullptr);
}