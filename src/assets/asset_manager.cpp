#include "asset_manager.h"

#include <loggers/logger.h>

#include <iostream>

using namespace Entropy::Assets;
using namespace Entropy::Graphics::Vulkan::Textures;

AssetManager::~AssetManager() {
  // Delete all assets
  for (const auto& [key, value] : assets_) {
    if (value.type == kTexture) {
      delete static_cast<Texture*>(value.asset);
    } else if (value.type == kTextureAtlas) {
      delete static_cast<TextureAtlas*>(value.asset);
    }
  }
  assets_.clear();
}

IAssetManager::AssetHandle AssetManager::LoadTexture(const std::string& path) {
  if (const auto it = assets_.find(path); it != assets_.end()) {
    return it->second;
  }
  assets_[path] = AssetHandle{new Texture(path), asset_index++, path, false, kTexture};
  return assets_[path];
}

IAssetManager::AssetHandle AssetManager::LoadTextureAsync(const std::string& path) {
  std::lock_guard lock(mutex_);
  if (const auto it = assets_.find(path); it != assets_.end()) {
    return it->second;
  }
  if (const auto fit = futures_.find(path);
    fit != futures_.end()) {
    return AssetHandle{nullptr, -1, path, true, kTexture};
  }

  const std::shared_future future = std::async(
      std::launch::async, [&, path] { return AssetHandle{new Texture(path), asset_index++, path, true, kTexture}; });

  futures_[path] = future;
  return AssetHandle{nullptr, -1, path, true, kTexture};
}

IAssetManager::AssetHandle AssetManager::LoadTextureAtlas(const std::string& path) {
  auto* atlas = static_cast<TextureAtlas*>(assets_[std::string("TextureAtlas" + std::to_string(asset_index))].asset);
  if (atlas == nullptr) {
    LOG_INFO(logger_, "Creating a new atlas..");
    atlas = new TextureAtlas();
    if (std::find(atlas->imagePaths_.begin(), atlas->imagePaths_.end(), path) == atlas->imagePaths_.end()) {
      LOG_INFO(logger_, "Adding image path to atlas..");
      atlas->imagePaths_.push_back(path);
    }
    atlas->CreateAtlas(atlas->imagePaths_);
    assets_[std::string("TextureAtlas" + std::to_string(asset_index))] = AssetHandle{atlas, asset_index++, path, false, kTextureAtlas};
  } else {
    if (atlas->imagePaths_.size() < 10) {
      if (std::find(atlas->imagePaths_.begin(), atlas->imagePaths_.end(), path) == atlas->imagePaths_.end()) {
        atlas->imagePaths_.push_back(path);
      }
      atlas->CreateAtlas(atlas->imagePaths_);
    } else {
      LOG_INFO(logger_, "Creating a new atlas since old one maxed out..");
      atlas = new TextureAtlas();
      if (std::find(atlas->imagePaths_.begin(), atlas->imagePaths_.end(), path) == atlas->imagePaths_.end()) {
        atlas->imagePaths_.push_back(path);
      }
      atlas->CreateAtlas(atlas->imagePaths_);
      assets_[std::string("TextureAtlas" + std::to_string(asset_index))] = AssetHandle{atlas, asset_index++, path, false, kTextureAtlas};
    }
  }
  atlas->DebugPrint(std::string("TextureAtlas" + std::to_string(asset_index)));
  return assets_[std::string("TextureAtlas" + std::to_string(asset_index))];
}

IAssetManager::AssetHandle AssetManager::LoadTextureAtlasAsync(const std::string& path) {

}

/*
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
*/
