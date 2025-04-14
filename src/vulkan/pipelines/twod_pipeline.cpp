#include "twod_pipeline.h"

#include <iostream>

#include "config.h"

#if ENTROPY_PLATFORM == IOS
#include <CoreFoundation/CoreFoundation.h>

static std::string GetProjectBasePath() {

  const CFURLRef resourceURL =
      CFBundleCopyResourcesDirectoryURL(CFBundleGetMainBundle());
  if (char resourcePath[PATH_MAX]; CFURLGetFileSystemRepresentation(
          resourceURL, true, reinterpret_cast<UInt8*>(resourcePath),
          PATH_MAX)) {
    if (resourceURL != nullptr) {
      CFRelease(resourceURL);
    }
    return resourcePath;
  }
  return "";
}
#endif

using namespace Entropy::Graphics::Vulkan::Pipelines;

TwoDPipeline::TwoDPipeline(
    const std::shared_ptr<RenderPasses::RenderPass>& renderPass)
    : BasePipeline(renderPass) {

  VkDescriptorSetLayoutBinding uboLayoutBinding{};
  uboLayoutBinding.binding = 1;
  uboLayoutBinding.descriptorCount = 1;
  uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  uboLayoutBinding.pImmutableSamplers = nullptr;
  uboLayoutBinding.stageFlags = VK_SHADER_STAGE_ALL;

  VkDescriptorSetLayoutBinding samplerLayoutBinding{};
  samplerLayoutBinding.binding = 2;
  samplerLayoutBinding.descriptorCount = 1;
  samplerLayoutBinding.descriptorType =
      VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  samplerLayoutBinding.pImmutableSamplers = nullptr;
  samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

  const std::vector bindings = {uboLayoutBinding,
                                samplerLayoutBinding};

  // const std::vector bindings2 = {samplerLayoutBinding};

  const std::vector<VkDescriptorBindingFlags> bindingFlags0 = {0, VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT};
  // const std::vector<VkDescriptorBindingFlags> bindingFlags1 = {};

  descriptorSetLayouts_.push_back(
      std::make_shared<DescriptorSetLayout>(bindings, bindingFlags0));
  /*
descriptorSetLayouts_.push_back(
    std::make_shared<DescriptorSetLayout>(bindings2, bindingFlags1));
    */

#if ENTROPY_PLATFORM == IOS
  shader_ = std::make_shared<Shaders::Shader>(
      GetProjectBasePath() + "/2dshader_vert.spv",
      GetProjectBasePath() + "/2dshader_frag.spv");
#else
  shader_ = std::make_shared<Shaders::Shader>("2dshader_vert.spv",
                                              "2dshader_frag.spv");
#endif

  std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
  for (const auto& descriptorSetLayout : descriptorSetLayouts_) {
    descriptorSetLayouts.push_back(descriptorSetLayout->Get());
  }

  VkDescriptorSetAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  allocInfo.descriptorPool = descriptorPool_->Get();
  allocInfo.descriptorSetCount = 1;
  allocInfo.pSetLayouts = descriptorSetLayouts.data();

  VK_CHECK(vkAllocateDescriptorSets(logicalDevice_->Get(), &allocInfo,
                                    &descriptorSet));

  Build();
}
