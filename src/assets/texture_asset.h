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

#ifndef ENTROPY_ASSETS_TEXTURE_ASSET_H
#define ENTROPY_ASSETS_TEXTURE_ASSET_H

#include <string>
#include "iasset.h"
#include "loggers/logger.h"
#include "vulkan/textures/texture.h"

namespace Entropy::Assets {
class TextureAsset : public IAsset {
 public:
  TextureAsset() {
    texture = std::make_shared<Entropy::Vulkan::Textures::Texture>();
  }

  void AddLayer(const std::string& path) {
    paths_.emplace_back(path);
    texture->AddLayer(path, 512, 512);
    index++;
  }

  const std::string& GetName() const override { return name_; }
  std::shared_ptr<Entropy::Vulkan::Textures::Texture> texture;
  int32_t index = -1;

 private:
  std::string name_;
  std::vector<std::string> paths_;
};
}  // namespace Entropy::Assets

#endif  // ENTROPY_ASSETS_TEXTURE_ASSET_H
