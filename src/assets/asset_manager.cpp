#include "asset_manager.h"

#include <cstdint>
#include <iostream>

using namespace Entropy::Assets;
using namespace Entropy::Graphics::Vulkan::Textures;

int32_t AssetManager::LoadTexture(const std::string& path) {
  auto it = textures_.find(textureIndex_);
  if (it != textures_.end()) {
    return it->first;
  }
  std::shared_ptr<Texture> asset = std::make_shared<Texture>(path);
  textures_[textureIndex_] = asset;
  return textureIndex_++;
}

std::shared_ptr<Texture> AssetManager::GetTexture(const int32_t textureId) {
  auto it = textures_.find(textureId);
  return (it != textures_.end()) ? it->second : nullptr;
}

int32_t AssetManager::LoadTextureAsync(const std::string& path) {
  std::lock_guard lock(mutex_);

  // Check if texture is already loaded
  if (const auto it = textures_.find(path); it != textures_.end()) {
    return it->second->textureIndex;
  }

  // Check if it's already loading
  if (const auto fit = textureFutures_.find(path);
      fit != textureFutures_.end()) {
    return {};
  }

  // Start async loading using shared_future
  std::shared_future future = std::async(std::launch::async, [path]() {
    return std::make_shared<Graphics::Vulkan::Textures::Texture>(path);
  });

  // Store the shared_future
  textureFutures_[path] = std::move(future);
  textureIndex_++;
  return textureIndex_;
}

std::shared_ptr<Texture> AssetManager::GetTextureAsync(
    const int32_t textureId) {
  std::lock_guard lock(mutex_);

  // Check if texture is already loaded
  if (const auto it = textures_.find(textureId); it != textures_.end()) {
    return it->second;
  }

  // Check if it's still loading
  if (const auto fit = textureFutures_.find(textureId);
      fit != textureFutures_.end()) {
    auto asset = fit->second.get();  // Safe to call multiple times
    textures_[textureId] = asset;    // Store in the loaded map
    textureFutures_.erase(fit);      // Remove from future map
    return asset;
  }

  return nullptr;  // Asset not found
}

void AssetManager::UnloadTexture(const int32_t textureId) {
  textures_.erase(textureId);
}
