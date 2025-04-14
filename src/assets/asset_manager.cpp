#include "asset_manager.h"
#include "loggers/logger.h"
#include "vulkan/textures/texture.h"
#include "vulkan/textures/textureatlas.h"

using namespace Entropy::Assets;
using namespace Entropy::Graphics::Vulkan::Textures;

AssetManager::~AssetManager() {
  for (const auto& [key, value] : assets_) {
    if (value.type == kTexture) {
      delete static_cast<Texture*>(value.asset);
    } else if (value.type == kTextureAtlas) {
      LOG_INFO(logger_, "Deleting texture atlas..");
      delete static_cast<TextureAtlas*>(value.asset);
    }
  }
  assets_.clear();
}

std::vector<IAssetManager::AssetHandle> AssetManager::LoadTexture(const std::vector<std::string> &paths) {
  std::vector<AssetHandle> assets;
  for (const auto& path : paths) {
    if (const auto it = assets_.find(path); it != assets_.end()) {
      assets.push_back(it->second);
      continue;
    }
    assets_[path] = AssetHandle{new Texture(path), asset_index++, path.c_str(), false, kTexture};
    assets.push_back(assets_[path]);
  }
  return assets;
}

std::vector<IAssetManager::AssetHandle> AssetManager::LoadTextureAsync(const std::vector<std::string> &paths) {
  /*
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
  */
  return {};
}

/*
std::vector<IAssetManager::AssetHandle> AssetManager::LoadTextureAtlas(const std::vector<std::string> &paths) {
  auto* atlas = static_cast<TextureAtlas*>(assets_[std::string("TextureAtlas" + std::to_string(asset_index))].asset);
  if (atlas == nullptr) {
    LOG_INFO(logger_, "Creating a new atlas..");
    atlas = new TextureAtlas();
    for (auto &path : paths) {
      if (std::find(atlas->imagePaths_.begin(), atlas->imagePaths_.end(), path) == atlas->imagePaths_.end()) {
        LOG_INFO(logger_, "Adding image path to atlas..");
        atlas->imagePaths_.push_back(path);
      }
    }
    atlas->CreateAtlas(atlas->imagePaths_);
    assets_[std::string("TextureAtlas" + std::to_string(asset_index))] = AssetHandle{atlas, asset_index++, "", false, kTextureAtlas};
  } else {
    if (atlas->imagePaths_.size() < 2) {
      for (auto &path : paths) {
        if (std::find(atlas->imagePaths_.begin(), atlas->imagePaths_.end(), path) == atlas->imagePaths_.end()) {
          LOG_INFO(logger_, "Adding image path to atlas..");
          atlas->imagePaths_.push_back(path);
        }
      }
      atlas->CreateAtlas(atlas->imagePaths_);
    } else {
      LOG_INFO(logger_, "Creating a new atlas since old one maxed out..");
      atlas = new TextureAtlas();
      for (auto &path : paths) {
        if (std::find(atlas->imagePaths_.begin(), atlas->imagePaths_.end(), path) == atlas->imagePaths_.end()) {
          LOG_INFO(logger_, "Adding image path to atlas..");
          atlas->imagePaths_.push_back(path);
        }
      }
      atlas->CreateAtlas(atlas->imagePaths_);
      assets_[std::string("TextureAtlas" + std::to_string(asset_index))] = AssetHandle{atlas, asset_index++, "", false, kTextureAtlas};
    }
  }
  atlas->DebugPrint(std::string("TextureAtlas" + std::to_string(asset_index)));
  return std::vector{assets_[std::string("TextureAtlas" + std::to_string(asset_index))]};
}
*/


std::vector<IAssetManager::AssetHandle> AssetManager::LoadTextureAtlas(const std::vector<std::string>& paths) {
  const std::string key = "TextureAtlas" + std::to_string(asset_index);
  LOG_INFO(logger_, "Creating a new atlas..");

  auto* atlas = new TextureAtlas();
  atlas->image_paths = paths;

  for (const auto& path : paths) {
    LOG_INFO(logger_, "Adding image path to atlas: {}", path);
  }

  atlas->CreateAtlas();
  atlas->DebugPrint(key);

  assets_[key] = AssetHandle{atlas, asset_index++, "", false, kTextureAtlas};
  return {assets_[key]};
}

std::vector<IAssetManager::AssetHandle> AssetManager::LoadTextureAtlasAsync(
    const std::vector<std::string>& paths) {
  return {};
}
