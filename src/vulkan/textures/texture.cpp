#include "texture.h"
#include <vulkan/vulkan.h>
#include <iostream>
#include "config.h"
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"
#include "vulkan/buffers/staging_buffer.h"
#include "vulkan/utilities/helpers.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb/stb_image_resize2.h"

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

namespace Entropy::Graphics::Vulkan::Textures {

// Custom write callback to store PNG data in a stbi_uc* buffer
void write_callback(void* context, void* data, int size) {
  // Cast context to a stbi_uc pointer to store data
  stbi_uc** buffer = (stbi_uc**)context;

  // Allocate new memory safely
  stbi_uc* temp = (stbi_uc*)realloc(*buffer, size);
  if (temp != NULL) {
      *buffer = temp;
      memcpy(*buffer, data, size);
  } else {
      // Handle allocation failure
      // Leave *buffer unchanged; caller must deal with it
  }
}

Texture::Texture(const int32_t width, const int32_t height) {
  // Define the pixel data for the 1x1 transparent image (RGBA)
  constexpr uint8_t image_data[4] = {255, 255, 255, 0};

  // Allocate a stbi_uc* buffer to hold the PNG data (initially empty)
  stbi_uc* pixels = nullptr;

  // Create the PNG in memory (1x1 image, 3 channels, RGB color)
  stbi_write_png_to_func(write_callback, &pixels, width, height, 4, image_data,
                         0);

  const VkDeviceSize imageSize = width * height * 4;
  assert(pixels != nullptr);

  stagingBuffer_ = std::make_unique<StagingBuffer>(
      imageSize, pixels, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);

  // Create a buffer and free pixels
  free(pixels);

  Create(width, height);
}

Texture::Texture(const std::vector<uint8_t> &data, const int width, const int height) {
  const VkDeviceSize imageSize = width * height * 4;
  stagingBuffer_ = std::make_unique<StagingBuffer>(
      imageSize, data.data(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
  Create(width, height);
}

Texture::Texture(const std::string& path) {
  assert(!path.empty());

  int texWidth, texHeight, texChannels;
  stbi_set_flip_vertically_on_load(true);

  // Load the image pixels
#if ENTROPY_PLATFORM == IOS
  stbi_uc* pixels =
      stbi_load((GetProjectBasePath() + "/" + path).c_str(), &texWidth,
                &texHeight, &texChannels, STBI_rgb_alpha);
#else
  stbi_uc* pixels = stbi_load((path).c_str(), &texWidth, &texHeight,
                              &texChannels, STBI_rgb_alpha);
#endif

  const VkDeviceSize imageSize = texWidth * texHeight * 4;
  assert(pixels != nullptr);

  stagingBuffer_ = std::make_unique<StagingBuffer>(
      imageSize, pixels, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);

  // Create a buffer and free pixels
  stbi_image_free(pixels);

  Create(texWidth, texHeight);
}

/*
    explicit Texture(const FT_Bitmap &bitmap) {
    const uint32_t texWidth = bitmap.width;
    const uint32_t texHeight = bitmap.rows;

    const VkDeviceSize imageSize = texWidth * texHeight;

    _stagingBuffer = std::make_shared<StagingBuffer>(
        imageSize, bitmap.buffer, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);

    // Get the color format being used
    constexpr VkFormat colorFormat =
        VK_FORMAT_R8_UNORM;  // @TODO why is this not the same as other color
    // formats?

    // // Create, transition and copy the image
    CreateImage(texWidth, texHeight, colorFormat, VK_IMAGE_TILING_OPTIMAL,
                VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                _textureImage);

    TransitionImageLayout(_textureImage, VK_IMAGE_LAYOUT_UNDEFINED,
                            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    CopyBufferToImage(_stagingBuffer->GetVulkanBuffer(), _textureImage,
                        texWidth, texHeight);

    TransitionImageLayout(_textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    imageView = std::make_shared<ImageView>(_textureImage, colorFormat);
    BindDescriptorSet();
    }
    */

void Texture::Create(const int width, const int height) {

  // @TODO why is this not the same as in getColorFormat?
  constexpr VkFormat colorFormat = VK_FORMAT_R8G8B8A8_UNORM;

  CreateImage(width, height, colorFormat, VK_IMAGE_TILING_OPTIMAL,
              VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
              textureImage_);

  TransitionImageLayout(textureImage_, VK_IMAGE_LAYOUT_UNDEFINED,
                        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

  CopyBufferToImage(stagingBuffer_->GetVulkanBuffer(), textureImage_,
                    static_cast<uint32_t>(width),
                    static_cast<uint32_t>(height));

  TransitionImageLayout(textureImage_, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

  imageView_ = std::make_shared<ImageView>(textureImage_, colorFormat);

  CreateTextureSampler();
}

void Texture::CreateTextureSampler() {
  VkPhysicalDeviceProperties properties{};
  vkGetPhysicalDeviceProperties(physicalDevice_->Get(), &properties);

  VkSamplerCreateInfo samplerInfo{};
  samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
  samplerInfo.magFilter = VK_FILTER_LINEAR;
  samplerInfo.minFilter = VK_FILTER_LINEAR;

  samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.anisotropyEnable = VK_TRUE;
  samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
  samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
  samplerInfo.unnormalizedCoordinates = VK_FALSE;
  samplerInfo.compareEnable = VK_FALSE;
  samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
  samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
  samplerInfo.mipLodBias = 0.0f;
  samplerInfo.minLod = 0.0f;
  samplerInfo.maxLod = 0.0f;

  VK_CHECK(vkCreateSampler(logicalDevice_->Get(), &samplerInfo, nullptr,
                           &textureSampler_));
}

std::vector<uint8_t> Texture::LoadAndResize(const std::string &path,
                                            const uint32_t targetWidth, const uint32_t targetHeight) {
  int w, h, channels;
  uint8_t* data = stbi_load(path.c_str(), &w, &h, &channels, 4); // Force RGBA
  if (!data) throw std::runtime_error("Failed to load image");

  std::vector<uint8_t> resized(targetWidth * targetHeight * 4);
  stbir_resize_uint8_linear(
      data,
      static_cast<int32_t>(targetWidth), static_cast<int32_t>(targetHeight), 0,
      resized.data(),
      0, 0, 0,
      STBIR_RGBA
  );
  stbi_image_free(data);
  return resized;
}
}  // namespace Entropy::Graphics::Vulkan::Textures
