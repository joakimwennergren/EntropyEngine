#ifndef ENTROPY_ASSETS_TEXTURE_MANAGER_H
#define ENTROPY_ASSETS_TEXTURE_MANAGER_H

#include <string>
#include <future>
#include <mutex>
#include <unordered_map>
#include "iasset_manager.h"

#include <iostream>

namespace Entropy::Assets {

class AssetManager final : public ServiceBase<IAssetManager> {
protected:
  std::shared_ptr<Graphics::Vulkan::Textures::Texture> LoadTexture(const std::string& path) override {
  	textureIndex_++;
    auto texture = LoadAsset(textures_, path);
	texture->textureIndex = textureIndex_;
    texture->texturePath = path;
	return texture;
  }

  int32_t  LoadTextureAsync(const std::string& path) override {
    std::lock_guard lock(mutex_);

    // Check if texture is already loaded
    if (const auto it = textures_.find(path); it != textures_.end()) {
      return it->second->textureIndex;
    }

    // Check if it's already loading
    if (const auto fit = textureFutures_.find(path); fit != textureFutures_.end()) {
      return {};
    }

    // Start async loading using shared_future
    std::shared_future future =
        std::async(std::launch::async, [path]() {
            return std::make_shared<Graphics::Vulkan::Textures::Texture>(path);
        });

    // Store the shared_future
    textureFutures_[path] = std::move(future);

    return -1;
  }

  std::shared_ptr<Graphics::Vulkan::Textures::Texture> GetTexture(const std::string& path) override {
        return GetAsset(textures_, path);
  }

  std::shared_ptr<Graphics::Vulkan::Textures::Texture> GetTextureAsync(const std::string& path) override {
    std::lock_guard lock(mutex_);

    // Check if texture is already loaded
    if (const auto it = textures_.find(path); it != textures_.end()) {
      return it->second;
    }

    // Check if it's still loading
    if (const auto fit = textureFutures_.find(path); fit != textureFutures_.end()) {
      auto asset = fit->second.get(); // Safe to call multiple times
      textures_[path] = asset;        // Store in the loaded map
      textureFutures_.erase(fit);     // Remove from future map
      return asset;
    }

    return nullptr;  // Asset not found
  }

  void UnloadTexture(const std::string& path) override {
    textures_.erase(path);
  }
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
  int32_t textureIndex_ = 0;

  std::mutex mutex_;
};

} // namespace Entropy::Assets

#endif // ENTROPY_ASSETS_TEXTURE_MANAGER_H
