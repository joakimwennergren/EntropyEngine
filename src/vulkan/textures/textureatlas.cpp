#include <fstream>
#include "config.h"
#include "filesystem/filesystem.h"
#include "loggers/logger.h"
#include "nlohmann/json.hpp"

#define STB_RECT_PACK_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"
#include "textureatlas.h"

using namespace Entropy::Vulkan::Textures;
using json = nlohmann::json;

TextureAtlas::TextureAtlas(std::vector<std::string>& paths) {
  paths_ = paths;
  pixel_buffers_.resize(paths_.size());
  regions.resize(paths_.size());
  stbi_set_flip_vertically_on_load(true);

  for (int32_t i = 0; i < paths_.size(); ++i) {
    int w, h, c;
#if ENTROPY_PLATFORM == IOS
    pixel_buffers[i] = stbi_load(
        (GetProjectBasePath() + "/" + paths_[i]).c_str(), &w, &h, &c, 4);
#else
    pixel_buffers_[i] = stbi_load(paths_[i].c_str(), &w, &h, &c, 4);
#endif
    if (!pixel_buffers_[i]) {
      return;
    }
    stbrp_rect rect = {};
    rect.id = i;
    rect.w = static_cast<stbrp_coord>(w);
    rect.h = static_cast<stbrp_coord>(h);
    rects_.push_back(rect);
  }
}

TextureAtlas::TextureAtlas(std::vector<FT_Bitmap>& bitmaps) {
  pixel_buffers_.resize(bitmaps.size());
  regions.resize(paths_.size());
  for (int32_t i = 0; i < bitmaps.size(); ++i) {
    stbrp_rect rect = {};
    rect.id = i;
    rect.w = static_cast<stbrp_coord>(bitmaps[i].width);
    rect.h = static_cast<stbrp_coord>(bitmaps[i].rows);
    rects_.push_back(rect);
  }
}

void TextureAtlas::Save(const std::string& name) const {
  stbi_write_png((name + ".png").c_str(), width_, height_, 4, atlas_.data(),
                 width_ * 4);
  json j;
  j["textureAtlas"] = {{"name", name}, {"width", width_}, {"height", height_}};
  for (const auto& region : regions) {
    j["textureAtlas"]["regions"].push_back({
        {"uMin", region.uMin},
        {"vMin", region.vMin},
        {"uMax", region.uMax},
        {"vMax", region.vMax},
    });
  }
  std::ofstream outFile(name + ".json");
  if (outFile.is_open()) {
    outFile << j.dump(4);
    outFile.close();
  }
}

bool TextureAtlas::Create() {

  auto roundToPowerOf2 = [](int size) {
    int power = 1;
    while (power < size) {
      power *= 2;
    }
    return power;
  };

  width_ = roundToPowerOf2(1000);
  height_ = roundToPowerOf2(1000);

  atlas_.resize(width_ * height_ * 4);
  std::vector<stbrp_node> nodes(width_);

  stbrp_init_target(&packer_, width_, height_, nodes.data(),
                    static_cast<uint32_t>(nodes.size()));
  stbrp_pack_rects(&packer_, rects_.data(),
                   static_cast<uint32_t>(rects_.size()));

  for (auto& [id, w, h, x, y, was_packed] : rects_) {
    if (!was_packed) {
      continue;
    }
    const unsigned char* img = pixel_buffers_[id];
    for (int row = 0; row < h; ++row) {
      std::memcpy(&atlas_[((y + row) * width_ + x) * 4], &img[row * w * 4],
                  w * 4);
    }
    regions[id] =
        TextureRegion({x / (float)width_, y / (float)height_,
                       (x + w) / (float)width_, (y + h) / (float)height_});
    stbi_image_free((void*)img);
  }

  texture = std::make_shared<Texture>(atlas_, width_, height_);
  return true;
}
