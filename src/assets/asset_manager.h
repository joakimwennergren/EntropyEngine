#ifndef ENTROPY_ASSETS_TEXTURE_MANAGER_H
#define ENTROPY_ASSETS_TEXTURE_MANAGER_H

#include <string>
#include <future>
#include <mutex>
#include <unordered_map>
#include "iasset_manager.h"

namespace Entropy::Assets {

class AssetManager final : public ServiceBase<IAssetManager> {
protected:
  int32_t LoadTexture(const std::string& path) override;
  std::shared_ptr<Graphics::Vulkan::Textures::Texture> GetTexture(const std::string& path) override;

  int32_t LoadTextureAsync(const std::string& path) override;
  std::shared_ptr<Graphics::Vulkan::Textures::Texture> GetTextureAsync(const std::string& path) override;

  void UnloadTexture(const std::string& path) override;
private:

  template <typename T>
  std::shared_ptr<T> LoadAsset(std::unordered_map<std::string, std::shared_ptr<T>>& assetMap, const std::string& path) {
    auto it = assetMap.find(path);
    if (it != assetMap.end()) {
      return it->second;
    }
    std::shared_ptr<T> asset = std::make_shared<T>(path);
    assetMap[path] = asset;
    return asset;
  }

  template <typename T>
	std::shared_ptr<T> GetAsset(std::unordered_map<std::string, std::shared_ptr<T>>& assetMap, const std::string& path) {
    auto it = assetMap.find(path);
    return (it != assetMap.end()) ? it->second : nullptr;
	}

  std::unordered_map<std::string, std::shared_future<std::shared_ptr<Graphics::Vulkan::Textures::Texture>>> textureFutures_;
  std::unordered_map<std::string, std::shared_ptr<Graphics::Vulkan::Textures::Texture>> textures_;
  int32_t textureIndex_ = -1;
  std::mutex mutex_;
};

} // namespace Entropy::Assets

#endif // ENTROPY_ASSETS_TEXTURE_MANAGER_H
