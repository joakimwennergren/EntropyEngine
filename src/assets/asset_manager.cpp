#include "asset_manager.h"

using namespace Entropy::Assets;
using namespace Entropy::Graphics::Vulkan::Textures;

int32_t AssetManager::LoadTexture(const std::string& path) {

  if (const auto it = textures_.find(path); it != textures_.end()) {
    return it->second.first;
  }

  auto asset = std::make_shared<Texture>(path);
  textureIndex_++;
  textures_[path] = std::make_pair(textureIndex_, asset);
  return textureIndex_;
}

std::shared_ptr<Texture> AssetManager::GetTexture(const int32_t textureId) {
  const auto it = textures_.find(GetTextureKeyById(textureId));
  return it != textures_.end() ? it->second.second : nullptr;
}

int32_t AssetManager::LoadTextureAsync(const std::string& path) {
  std::lock_guard lock(mutex_);

  // Check if texture is already loaded
  if (const auto it = textures_.find(path); it != textures_.end()) {
    return it->second.first;
  }

  // Check if it's already loading
  if (const auto fit = textureFutures_.find(path);
      fit != textureFutures_.end()) {
    return -1;
  }

  // Start async loading using shared_future
  std::shared_future<std::shared_ptr<Texture>> future = std::async(
      std::launch::async, [path] { return std::make_shared<Texture>(path); });

  // Store the shared_future
  textureIndex_++;
  textureFutures_[path] = std::make_pair(textureIndex_, future);
  return textureIndex_;
}

std::shared_ptr<Texture> AssetManager::GetTextureAsync(
    const int32_t textureId) {
  std::lock_guard lock(mutex_);

  // Check if texture is already loaded

  if (const auto it = textures_.find(GetTextureKeyById(textureId));
      it != textures_.end()) {
    return it->second.second;
  }

  for (const auto& [key, value] : textureFutures_) {
    if (value.first == textureId) {
      if (auto asset = value.second.get()) {
        // Ensure the asset is valid (not nullptr)
        textures_[key] = std::make_pair(textureId, asset);
        textureFutures_.erase(key);  // Remove from future map
        return asset;
      }
    }

    return nullptr;
  }

  return nullptr;
}

void AssetManager::UnloadTexture(const int32_t textureId) {
  textures_.erase(GetTextureKeyById(textureId));
}

std::string AssetManager::GetTextureKeyById(int32_t textureId) {
  for (const auto& [key, value] : textures_) {
    if (value.first == textureId) {
      return key;
    }
  }
  return {};  // or throw / return std::optional
}
