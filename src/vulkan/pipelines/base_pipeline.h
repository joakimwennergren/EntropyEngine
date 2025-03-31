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

#ifndef ENTROPY_VULKAN_BASE_PIPELINE_H
#define ENTROPY_VULKAN_BASE_PIPELINE_H

#include "vulkan/data/pushcontants.h"
#include "vulkan/data/vertex.h"
#include "vulkan/descriptorpools/descriptorpool.h"
#include "vulkan/descriptorsets/descriptorset.h"
#include "vulkan/pipelinecaches/pipeline_cache.h"
#include "vulkan/renderpasses/renderpass.h"
#include "vulkan/shaders/shader.h"
#include "vulkan/swapchains/swapchain.h"

namespace Entropy::Graphics::Vulkan::Pipelines {

template <class T> class BasePipeline {

public:
  BasePipeline(const std::shared_ptr<RenderPasses::RenderPass> &renderPass);
  ~BasePipeline();

  void Build();

  [[nodiscard]] VkPipeline GetPipeline() const { return pipeline_; };
  [[nodiscard]] VkPipelineLayout GetPipelineLayout() const {
    return pipelineLayout_;
  };

  VkDescriptorSet descriptorSet;

protected:
  VkPipelineLayout pipelineLayout_ = VK_NULL_HANDLE;
  VkPipeline pipeline_ = VK_NULL_HANDLE;
  std::shared_ptr<Shaders::Shader> shader_;
  std::shared_ptr<Devices::ILogicalDevice> logicalDevice_;
  std::shared_ptr<SwapChains::ISwapChain> swapChain_;
  std::shared_ptr<RenderPasses::RenderPass> renderPass_;
  std::shared_ptr<DescriptorPools::IDescriptorPool> descriptorPool_;
  std::shared_ptr<IPipelineCache> pipelineCache_;
  std::vector<std::shared_ptr<DescriptorSetLayout>> descriptorSetLayouts_;
};
} // namespace Entropy::Graphics::Vulkan::Pipelines

#endif // ENTROPY_VULKAN_BASE_PIPELINE_H