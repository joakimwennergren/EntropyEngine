#include "base_pipeline.h"
#include "vulkan/utilities/helpers.h"

using namespace Entropy::Vulkan::Devices;
using namespace Entropy::Vulkan::DescriptorSets;
using namespace Entropy::Vulkan::DescriptorPools;
using namespace Entropy::Vulkan::SwapChains;
using namespace Entropy::Vulkan::RenderPasses;
using namespace Entropy::Vulkan::Shaders;
using namespace Entropy::Vulkan::Data;

namespace Entropy::Vulkan::Pipelines {

template <class T>
BasePipeline<T>::BasePipeline(const std::shared_ptr<RenderPass>& renderPass)
    : descriptorSet(nullptr) {
  const ServiceLocator* sl = ServiceLocator::GetInstance();
  logicalDevice_ = sl->GetService<ILogicalDevice>();
  swapChain_ = sl->GetService<ISwapChain>();
  descriptorPool_ = sl->GetService<IDescriptorPool>();
  pipelineCache_ = sl->GetService<Caches::IPipelineCache>();
  renderPass_ = renderPass;
}

template <class T>
BasePipeline<T>::~BasePipeline() {
  vkDeviceWaitIdle(logicalDevice_->Get());
  vkDestroyPipeline(logicalDevice_->Get(), pipeline_, nullptr);
  vkDestroyPipelineLayout(logicalDevice_->Get(), pipelineLayout_, nullptr);
}

template <class T>
void BasePipeline<T>::Build() {
  VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
  vertShaderStageInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
  vertShaderStageInfo.module = shader_->GetVertShaderModule();
  vertShaderStageInfo.pName = "main";

  VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
  fragShaderStageInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  fragShaderStageInfo.module = shader_->GetFragShaderModule();
  fragShaderStageInfo.pName = "main";
  VkPipelineShaderStageCreateInfo shaderStages[2] = {vertShaderStageInfo,
                                                     fragShaderStageInfo};

  VkDynamicState dynamicStates[2] = {VK_DYNAMIC_STATE_VIEWPORT,
                                     VK_DYNAMIC_STATE_SCISSOR};
  VkPipelineDynamicStateCreateInfo dynamicState{};
  dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  dynamicState.dynamicStateCount = 2;
  dynamicState.pDynamicStates = dynamicStates;

  VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
  inputAssembly.sType =
      VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  // inputAssembly.topology = polygonMode == VK_POLYGON_MODE_LINE
  //                             ? VK_PRIMITIVE_TOPOLOGY_LINE_LIST
  //                             : VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  inputAssembly.primitiveRestartEnable = VK_FALSE;

  auto bindingDescriptions = T::getBindingDescriptions();
  auto attributeDescriptions = T::getAttributeDescriptions();

  VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
  vertexInputInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertexInputInfo.vertexBindingDescriptionCount =
      static_cast<uint32_t>(bindingDescriptions.size());
  vertexInputInfo.vertexAttributeDescriptionCount =
      static_cast<uint32_t>(attributeDescriptions.size());
  vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();
  vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

  std::vector<VkPipelineVertexInputStateCreateInfo> vertexInputStates(1);
  vertexInputStates[0] = vertexInputInfo;

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(swapChain_->swapChainExtent.width);
  viewport.height = static_cast<float>(swapChain_->swapChainExtent.height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = swapChain_->swapChainExtent;

  VkPipelineViewportStateCreateInfo viewportState{};
  viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewportState.viewportCount = 1;
  viewportState.pViewports = &viewport;
  viewportState.scissorCount = 1;
  viewportState.pScissors = &scissor;

  VkPipelineRasterizationStateCreateInfo rasterizer{};
  rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterizer.depthClampEnable = VK_FALSE;
  rasterizer.rasterizerDiscardEnable = VK_FALSE;
  rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
  rasterizer.lineWidth = 1.0f;
  rasterizer.cullMode = VK_CULL_MODE_NONE;
  rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
  rasterizer.depthBiasEnable = VK_FALSE;
  rasterizer.depthBiasConstantFactor = 1.25f;
  rasterizer.depthBiasClamp = 0.0f;
  rasterizer.depthBiasSlopeFactor = 1.75f;

  VkPipelineMultisampleStateCreateInfo multisampling{};
  multisampling.sType =
      VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisampling.sampleShadingEnable = VK_FALSE;
  multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  multisampling.minSampleShading = 1.0f;
  multisampling.pSampleMask = nullptr;
  multisampling.alphaToCoverageEnable = VK_FALSE;
  multisampling.alphaToOneEnable = VK_FALSE;

  VkPipelineColorBlendAttachmentState colorBlendAttachment{};
  colorBlendAttachment.colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
      VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  colorBlendAttachment.blendEnable = VK_TRUE;
  colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
  colorBlendAttachment.dstColorBlendFactor =
      VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
  colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
  colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
  colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
  colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

  VkPipelineColorBlendStateCreateInfo colorBlending{};
  colorBlending.sType =
      VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  colorBlending.logicOpEnable = VK_FALSE;
  colorBlending.logicOp = VK_LOGIC_OP_COPY;
  colorBlending.attachmentCount = 1;
  colorBlending.pAttachments = &colorBlendAttachment;
  colorBlending.blendConstants[0] = 0.0f;
  colorBlending.blendConstants[1] = 0.0f;
  colorBlending.blendConstants[2] = 0.0f;
  colorBlending.blendConstants[3] = 0.0f;

  // VkPushConstantRange push_constant;
  // push_constant.offset = 0;
  // push_constant.size = sizeof(PushConstant);
  // push_constant.stageFlags = VK_SHADER_STAGE_ALL;

  std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
  for (auto& descriptorSetLayout : descriptorSetLayouts_) {
    descriptorSetLayouts.push_back(descriptorSetLayout->Get());
  }

  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount =
      static_cast<uint32_t>(descriptorSetLayouts_.size());
  pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
  // pipelineLayoutInfo.pPushConstantRanges = &push_constant;
  // pipelineLayoutInfo.pushConstantRangeCount = 1;

  VK_CHECK(vkCreatePipelineLayout(logicalDevice_->Get(), &pipelineLayoutInfo,
                                  nullptr, &pipelineLayout_));

  VkPipelineDepthStencilStateCreateInfo depthStencil{};
  depthStencil.sType =
      VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  depthStencil.depthTestEnable = VK_TRUE;
  depthStencil.depthWriteEnable = VK_TRUE;
  depthStencil.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
  depthStencil.depthBoundsTestEnable = VK_FALSE;
  depthStencil.minDepthBounds = 0.0f;
  depthStencil.maxDepthBounds = 1.0f;
  depthStencil.stencilTestEnable = VK_FALSE;
  depthStencil.front = {};
  depthStencil.back = {};

  VkGraphicsPipelineCreateInfo pipelineInfo{};
  pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipelineInfo.stageCount = 2;
  pipelineInfo.pStages = shaderStages;
  pipelineInfo.pVertexInputState = vertexInputStates.data();
  pipelineInfo.pInputAssemblyState = &inputAssembly;
  pipelineInfo.pViewportState = &viewportState;
  pipelineInfo.pRasterizationState = &rasterizer;
  pipelineInfo.pMultisampleState = &multisampling;
  pipelineInfo.pDepthStencilState = &depthStencil;
  pipelineInfo.pColorBlendState = &colorBlending;
  pipelineInfo.pDynamicState = &dynamicState;
  pipelineInfo.layout = pipelineLayout_;
  pipelineInfo.renderPass = renderPass_->Get();
  pipelineInfo.subpass = 0;
  pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
  pipelineInfo.basePipelineIndex = -1;

  VK_CHECK(vkCreateGraphicsPipelines(logicalDevice_->Get(),
                                     pipelineCache_->Get(), 1, &pipelineInfo,
                                     nullptr, &pipeline_));
}

template class BasePipeline<TwoDVertex>;
//template class BasePipeline<ThreeDAnimVertex>;

}  // namespace Entropy::Vulkan::Pipelines
