#include "renderpass.h"

#include <iostream>

#include "config.h"
#include "vulkan/utilities/helpers.h"

using namespace Entropy::Vulkan::RenderPasses;
using namespace Entropy::Vulkan::Textures;
using namespace Entropy::Vulkan::Devices;
using namespace Entropy::Vulkan::SwapChains;

RenderPass::RenderPass() {
  const ServiceLocator* sl = ServiceLocator::GetInstance();
  logicalDevice_ = sl->GetService<ILogicalDevice>();
  physicalDevice_ = sl->GetService<IPhysicalDevice>();
  swapChain_ = sl->GetService<ISwapChain>();

  VkAttachmentDescription depthAttachment{};
  depthAttachment.format = FindDepthFormat();
  depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  depthAttachment.finalLayout =
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  VkAttachmentReference depthAttachmentRef{};
  depthAttachmentRef.attachment = 1;
  depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  VkAttachmentDescription colorAttachment{};
  colorAttachment.format = COLOR_FORMAT;
  colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkSubpassDependency dependency{};
  dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                            VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                            VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
                             VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
  dependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

  VkAttachmentReference colorAttachmentRef{};
  colorAttachmentRef.attachment = 0;
  colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass{};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &colorAttachmentRef;
  subpass.pDepthStencilAttachment = &depthAttachmentRef;

  std::array<VkAttachmentDescription, 2> attachments = {colorAttachment,
                                                        depthAttachment};

  VkRenderPassCreateInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
  renderPassInfo.pAttachments = attachments.data();
  renderPassInfo.subpassCount = 1;
  renderPassInfo.pSubpasses = &subpass;
  renderPassInfo.dependencyCount = 1;
  renderPassInfo.pDependencies = &dependency;

  VK_CHECK(vkCreateRenderPass(logicalDevice_->Get(), &renderPassInfo, nullptr,
                              &renderPass_));
}

RenderPass::~RenderPass() {
  vkDeviceWaitIdle(logicalDevice_->Get());
  for (const auto framebuffer : frameBuffers_) {
    vkDestroyFramebuffer(logicalDevice_->Get(), framebuffer, nullptr);
  }
  vkDestroyRenderPass(logicalDevice_->Get(), renderPass_, nullptr);
}

void RenderPass::Begin(const std::shared_ptr<CommandBuffer>& commandBuffer,
                       const uint32_t imageIndex) {
  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = renderPass_;
  renderPassInfo.framebuffer = frameBuffers_[imageIndex];
  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = VkExtent2D{
      swapChain_->swapChainExtent.width, swapChain_->swapChainExtent.height};

  std::array<VkClearValue, 2> clearValues{};
  clearValues[0].color = {{0.0f, 0.2f, 0.0f, 1.0f}};
  clearValues[1].depthStencil = {1.0f, 0};

  renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
  renderPassInfo.pClearValues = clearValues.data();

  vkCmdBeginRenderPass(commandBuffer->Get(), &renderPassInfo,
                       VK_SUBPASS_CONTENTS_INLINE);
}

void RenderPass::End(std::shared_ptr<CommandBuffer>& commandBuffer) {
  vkCmdEndRenderPass(commandBuffer->Get());
}

void RenderPass::RecreateFrameBuffers(const uint32_t width,
                                      const uint32_t height) {
  for (const auto framebuffer : frameBuffers_) {
    vkDestroyFramebuffer(logicalDevice_->Get(), framebuffer, nullptr);
  }
  CreateFrameBuffers(width, height);
}

void RenderPass::RecreateDepthBuffer(uint32_t width, uint32_t height) {
  depthBufferTexture_ = std::make_shared<DepthBufferTexture>(width, height);
}

void RenderPass::CreateFrameBuffers(const uint32_t width,
                                    const uint32_t height) {
  swapChainTextures_.clear();
  frameBuffers_.clear();

  frameBuffers_.resize(swapChain_->swapChainImageViews.size());

  for (size_t i = 0; i < swapChain_->swapChainImageViews.size(); i++) {
    std::array<VkImageView, 2> attachments = {
        swapChain_->swapChainImageViews[i]->Get(),
        depthBufferTexture_->GetImageView()->Get()};

    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = renderPass_;
    framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    framebufferInfo.pAttachments = attachments.data();
    framebufferInfo.width = width;
    framebufferInfo.height = height;
    framebufferInfo.layers = 1;

    VK_CHECK(vkCreateFramebuffer(logicalDevice_->Get(), &framebufferInfo,
                                 nullptr, &frameBuffers_[i]));
  }
}

VkFormat RenderPass::findSupportedFormat(
    const std::vector<VkFormat>& candidates, const VkImageTiling tiling,
    const VkFormatFeatureFlags features) {
  for (VkFormat format : candidates) {
    VkFormatProperties props;
    vkGetPhysicalDeviceFormatProperties(physicalDevice_->Get(), format, &props);

    if (tiling == VK_IMAGE_TILING_LINEAR &&
        (props.linearTilingFeatures & features) == features) {
      return format;
    }

    if (tiling == VK_IMAGE_TILING_OPTIMAL &&
        (props.optimalTilingFeatures & features) == features) {
      return format;
    }
  }

  LOG_INFO(logger_, "Failed to find supported format");
  return VK_FORMAT_UNDEFINED;
}

VkFormat RenderPass::FindDepthFormat() {
  return findSupportedFormat(
      {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT,
       VK_FORMAT_D24_UNORM_S8_UINT},
      VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}
