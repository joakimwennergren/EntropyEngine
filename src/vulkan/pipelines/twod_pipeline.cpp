#include "twod_pipeline.h"
#include "config.h"

#if PLATFORM == IOS
#include <CoreFoundation/CoreFoundation.h>

static std::string GetProjectBasePath() {

  CFURLRef resourceURL =
      CFBundleCopyResourcesDirectoryURL(CFBundleGetMainBundle());
  char resourcePath[PATH_MAX];
  if (CFURLGetFileSystemRepresentation(resourceURL, true, (UInt8 *)resourcePath,
                                       PATH_MAX)) {
    if (resourceURL != NULL) {
      CFRelease(resourceURL);
    }
    return resourcePath;
  }
  return "";
}
#endif

using namespace Entropy::Graphics::Vulkan::Pipelines;

TwoDPipeline::TwoDPipeline(
    const std::shared_ptr<RenderPasses::RenderPass> &renderPass)
    : BasePipeline(renderPass) {

  VkDescriptorSetLayoutBinding instanceLayoutBinding = {};
  instanceLayoutBinding.binding = 0;
  instanceLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
  instanceLayoutBinding.descriptorCount = 1;
  instanceLayoutBinding.stageFlags = VK_SHADER_STAGE_ALL;
  instanceLayoutBinding.pImmutableSamplers = nullptr;

  VkDescriptorSetLayoutBinding uboLayoutBinding{};
  uboLayoutBinding.binding = 1;
  uboLayoutBinding.descriptorCount = 1;
  uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  uboLayoutBinding.pImmutableSamplers = nullptr;
  uboLayoutBinding.stageFlags = VK_SHADER_STAGE_ALL;

  VkDescriptorSetLayoutBinding samplerLayoutBinding{};
  samplerLayoutBinding.binding = 2;
  samplerLayoutBinding.descriptorCount = TEXTURE_ARRAY_SIZE;
  samplerLayoutBinding.descriptorType =
      VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  samplerLayoutBinding.pImmutableSamplers = nullptr;
  samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

  const std::vector bindings = {instanceLayoutBinding, uboLayoutBinding,
                                samplerLayoutBinding};

  // const std::vector bindings2 = {samplerLayoutBinding};

  const std::vector<VkDescriptorBindingFlags> bindingFlags0 = {
      0, 0, VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT};
  // const std::vector<VkDescriptorBindingFlags> bindingFlags1 = {};

  descriptorSetLayouts_.push_back(
      std::make_shared<DescriptorSetLayout>(bindings, bindingFlags0));
  /*
descriptorSetLayouts_.push_back(
    std::make_shared<DescriptorSetLayout>(bindings2, bindingFlags1));
    */

  #if PLATFORM == IOS
  shader_ = std::make_shared<Shaders::Shader>(GetProjectBasePath() + "/2d_shader_vert.spv",
                                              GetProjectBasePath() + "/2d_shader_frag.spv");
  #else
  shader_ = std::make_shared<Shaders::Shader>("2d_shader_vert.spv",
                                            "2d_shader_frag.spv");
  #endif

  std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
  for (const auto &descriptorSetLayout : descriptorSetLayouts_) {
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
