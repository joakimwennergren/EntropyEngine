#include "textureatlas.h"

#define STB_RECT_PACK_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"
#include "stb/stb_rect_pack.h"

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

using namespace Entropy::Graphics::Vulkan::Textures;

TextureAtlas::TextureAtlas() {}

bool TextureAtlas::CreateAtlas(const std::vector<std::string>& imagePaths) {

  stbi_set_flip_vertically_on_load(true);
  std::vector<stbrp_rect> rects;

  // Load images and store their dimensions
  std::vector<unsigned char*> images(imagePaths.size());
  int totalWidth = 0;
  int totalHeight = 0;
  int totalArea = 0;
  int maxWidth = 0;
  int maxHeight = 0;

  // Load images and calculate the total area
  for (int32_t i = 0; i < imagePaths.size(); ++i) {
    int w, h, c;
#if ENTROPY_PLATFORM == IOS
    images[i] = stbi_load((GetProjectBasePath() + "/" + imagePaths[i]).c_str(),
                          &w, &h, &c, 4);  // Force RGBA
#else
    images[i] = stbi_load(imagePaths[i].c_str(), &w, &h, &c, 4);  // Force RGBA
#endif
    if (!images[i]) {
      std::cerr << "Failed to load image: " << imagePaths[i] << "\n";
      return false;
    }

    totalWidth += w;
    totalHeight += h;
    totalArea += w * h;
    maxWidth = std::max(maxWidth, w);
    maxHeight = std::max(maxHeight, h);
    rects.push_back({i, w, h});
  }

  // Calculate initial atlas size based on total area
  int atlasWidth = std::sqrt(totalArea);
  atlasWidth = (atlasWidth == 0)
                   ? 1024
                   : atlasWidth;  // Ensure it's at least 1024 pixels wide
  atlasWidth = (atlasWidth % 2 == 0)
                   ? atlasWidth
                   : atlasWidth + 1;  // Ensure even number for better packing

  int atlasHeight = totalArea / atlasWidth;
  if (totalArea % atlasWidth != 0) {
    atlasHeight +=
        1;  // If it doesn't perfectly divide, add another row of space
  }

  // Round to nearest power of 2 for Vulkan efficiency
  auto roundToPowerOf2 = [](int size) {
    int power = 1;
    while (power < size) {
      power *= 2;
    }
    return power;
  };
  atlasWidth = roundToPowerOf2(atlasWidth);
  atlasHeight = roundToPowerOf2(atlasHeight);

  // Check if the initial atlas size fits, if not, progressively increase it
  while (atlasWidth * atlasHeight < totalArea) {
    atlasWidth *= 2;
    atlasHeight *= 2;
  }

  std::cout << "Adjusted Atlas Size (Power of 2): " << atlasWidth << "x"
            << atlasHeight << "\n";

  // Setup packing context
  stbrp_context packer;
  std::vector<stbrp_node> nodes(atlasWidth);
  stbrp_init_target(&packer, atlasWidth, atlasHeight, nodes.data(),
                    static_cast<uint32_t>(nodes.size()));

  // Pack rectangles
  if (!stbrp_pack_rects(&packer, rects.data(),
                        static_cast<uint32_t>(rects.size()))) {
    std::cerr << "Texture atlas packing failed!\n";
    return false;
  }

  // Create an empty atlas (RGBA)
  std::vector<unsigned char> atlas(atlasWidth * atlasHeight * 4,
                                   0);  // 4 channels (RGBA)

  // Copy images into the atlas
  for (auto& rect : rects) {
    if (!rect.was_packed)
      continue;

    int x = rect.x, y = rect.y, w = rect.w, h = rect.h;
    const unsigned char* img = images[rect.id];

    for (int row = 0; row < h; ++row) {
      std::memcpy(&atlas[((y + row) * atlasWidth + x) * 4], &img[row * w * 4],
                  w * 4);
    }

    // Store UV coordinates in textureRegions
    textureRegions.push_back({
        x / (float)atlasWidth,         // min U (left)
        y / (float)atlasHeight,        // min V (bottom)
        (x + w) / (float)atlasWidth,   // max U (right)
        (y + h) / (float)atlasHeight,  // max V (top)
        w,
        h  // width and height of the texture in pixels
    });

    stbi_image_free((void*)img);
  }

  texture_ = std::make_shared<Texture>(atlas, atlasWidth, atlasHeight);

  // Save atlas to file (optional, for debugging)
  //stbi_write_png(outputAtlasPath, atlasWidth, atlasHeight, 4, atlas.data(), atlasWidth * 4);

  //std::cout << "Texture atlas created: " << outputAtlasPath << "\n";
  return true;
}
