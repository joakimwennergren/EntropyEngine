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

#ifndef ENTROPY_ASSETS_IASSET_MANAGER_H
#define ENTROPY_ASSETS_IASSET_MANAGER_H

#include "servicelocators/servicelocator.h"
#include "vulkan/textures/texture.h"

namespace Entropy::Assets {
class IAssetManager : public IService {
 public:
  enum AssetType {
    kTexture,
    kTextureAtlas,
    kUnknown,
  };
  struct AssetHandle {
    void *asset{};
    int32_t index{};
    std::string paths;
    bool is_async = false;
    AssetType type = kUnknown;
  };
  enum LoadOperation {
    kLoadTextureSync,
    kLoadTextureAsync,
    kLoadTextureAtlasSync,
    kLoadTextureAtlasAsync,
  };
  std::vector<AssetHandle> Load(const std::vector<std::string> &paths, const LoadOperation operation) {
    if (operation == kLoadTextureSync) {
      return LoadTexture(paths);
    }
    if (operation == kLoadTextureAsync) {
      return LoadTextureAsync(paths);
    }
    if (operation == kLoadTextureAtlasSync) {
      return LoadTextureAtlas(paths);
    }
    return {};
  }
  ~IAssetManager() override = default;
 protected:
  virtual std::vector<AssetHandle> LoadTexture(const std::vector<std::string> &paths) = 0;
  virtual std::vector<AssetHandle> LoadTextureAsync(const std::vector<std::string> &paths) = 0;
  virtual std::vector<AssetHandle> LoadTextureAtlas(const std::vector<std::string> &paths) = 0;
  virtual std::vector<AssetHandle> LoadTextureAtlasAsync(const std::vector<std::string> &paths) = 0;
};
}  // namespace Entropy::Assets

#endif  // ENTROPY_ASSETS_IASSET_MANAGER_H
