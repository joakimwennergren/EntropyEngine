#ifndef ENTROPY_VULKAN_IPIPELINECACHE_H
#define ENTROPY_VULKAN_IPIPELINECACHE_H

#include <vulkan/vulkan.hpp>
#include "servicelocators/servicelocator.h"

class  IPipelineCache : public IService {
  public:
     ~IPipelineCache()override = default;
    virtual VkPipelineCache Get() = 0;
};

#endif // ENTROPY_VULKAN_IPIPELINECACHE_H
