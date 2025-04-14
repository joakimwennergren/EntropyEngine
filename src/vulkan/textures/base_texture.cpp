#include "base_texture.h"
#include "vulkan/utilities/helpers.h"

using namespace Entropy::Vulkan::Textures;

BaseTexture::BaseTexture() {
  const ServiceLocator* sl = ServiceLocator::GetInstance();
  physicalDevice_ = sl->getService<Devices::IPhysicalDevice>();
  logicalDevice_ = sl->getService<Devices::ILogicalDevice>();
  allocator_ = sl->getService<Memory::IAllocator>();
}

BaseTexture::~BaseTexture() {
  vmaDestroyImage(allocator_->Get(), textureImage_, allocation_);
  if (textureSampler_ != nullptr) {
    vkDestroySampler(logicalDevice_->Get(), textureSampler_, nullptr);
  }
}

void BaseTexture::TransitionImageLayout(const VkImage image,
                                        const VkImageLayout oldLayout,
                                        const VkImageLayout newLayout) {
  // Assert on parameters
  assert(image != VK_NULL_HANDLE);
  commandBuffer_ =
      std::make_unique<CommandBuffer>(VK_COMMAND_BUFFER_LEVEL_PRIMARY);
  commandBuffer_->RecordOnce();

  // Create pipeline barrier
  VkImageMemoryBarrier barrier{};
  barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  barrier.oldLayout = oldLayout;
  barrier.newLayout = newLayout;
  barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.image = image;
  barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  barrier.subresourceRange.baseMipLevel = 0;
  barrier.subresourceRange.levelCount = 1;
  barrier.subresourceRange.baseArrayLayer = 0;
  barrier.subresourceRange.layerCount = 1;

  VkPipelineStageFlags sourceStage = 0;
  VkPipelineStageFlags destinationStage = 0;

  if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
      newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

    sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
  } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
             newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
  } else {
    LOG_INFO(logger_, "Unsupported image layout transition!");
  }

  // Set the pipeline barrier
  vkCmdPipelineBarrier(commandBuffer_->Get(), sourceStage, destinationStage, 0,
                       0, nullptr, 0, nullptr, 1, &barrier);

  commandBuffer_->EndRecordingOnce();
  const auto cmdBuf = commandBuffer_->Get();

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &cmdBuf;
  VK_CHECK(vkQueueSubmit(logicalDevice_->GetGraphicQueue(), 1, &submitInfo,
                         nullptr));

  // @TODO Minimize these
  vkDeviceWaitIdle(logicalDevice_->Get());
}

void BaseTexture::CopyBufferToImage(const VkBuffer buffer, const VkImage image,
                                    const uint32_t width,
                                    const uint32_t height) {
  // Assert on parameters
  assert(image != VK_NULL_HANDLE);
  assert(width != 0);
  assert(height != 0);

  // Create a new command buffer and start one-time recording
  commandBuffer_ =
      std::make_unique<CommandBuffer>(VK_COMMAND_BUFFER_LEVEL_PRIMARY);
  commandBuffer_->RecordOnce();

  // Make the actual copy
  VkBufferImageCopy region{};
  region.bufferOffset = 0;
  region.bufferRowLength = 0;
  region.bufferImageHeight = 0;
  region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  region.imageSubresource.mipLevel = 0;
  region.imageSubresource.baseArrayLayer = 0;
  region.imageSubresource.layerCount = 1;
  region.imageOffset = {0, 0, 0};
  region.imageExtent = {width, height, 1};
  vkCmdCopyBufferToImage(commandBuffer_->Get(), buffer, image,
                         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

  commandBuffer_->EndRecordingOnce();
  const auto cmdBuf = commandBuffer_->Get();

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &cmdBuf;
  VK_CHECK(vkQueueSubmit(logicalDevice_->GetGraphicQueue(), 1, &submitInfo,
                         nullptr));

  // @todo remove this wait
  vkDeviceWaitIdle(logicalDevice_->Get());
}

void BaseTexture::CreateImage(const uint32_t width, const uint32_t height,
                              const VkFormat format, const VkImageTiling tiling,
                              const VkImageUsageFlags usage, VkImage& image) {
  // Assert on parameters
  assert(width != 0);
  assert(height != 0);
  assert(format != VK_FORMAT_UNDEFINED);

  VmaAllocationCreateInfo allocCreateInfo = {};
  allocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
  allocCreateInfo.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;

  VkImageCreateInfo imageInfo{};
  imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  imageInfo.imageType = VK_IMAGE_TYPE_2D;
  imageInfo.extent.width = width;
  imageInfo.extent.height = height;
  imageInfo.extent.depth = 1;
  imageInfo.mipLevels = 1;
  imageInfo.arrayLayers = 1;
  imageInfo.format = format;
  imageInfo.tiling = tiling;
  imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  imageInfo.usage = usage;
  imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
  imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  // Create the image
  VK_CHECK(vmaCreateImage(allocator_->Get(), &imageInfo, &allocCreateInfo,
                          &image, &allocation_, nullptr));
}
