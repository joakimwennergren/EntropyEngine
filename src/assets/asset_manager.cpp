#include "asset_manager.h"
#include "loggers/logger.h"
#include "vulkan/textures/texture.h"
#include "vulkan/textures/textureatlas.h"

using namespace Entropy::Assets;
using namespace Entropy::Vulkan::Textures;

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

std::vector<IAssetManager::AssetHandle> AssetManager::LoadTexture(
    const std::vector<std::string>& paths) {
  std::vector<AssetHandle> assets;
  for (const auto& path : paths) {
    if (const auto it = assets_.find(path); it != assets_.end()) {
      assets.push_back(it->second);
      continue;
    }
    assets_[path] = AssetHandle{new Texture(path), asset_index++, path.c_str(),
                                false, kTexture};
    assets.push_back(assets_[path]);
  }
  return assets;
}

std::vector<IAssetManager::AssetHandle> AssetManager::LoadTextureAsync(
    const std::vector<std::string>& paths) {
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

std::vector<IAssetManager::AssetHandle> AssetManager::LoadTextureAtlas(
    const std::vector<std::string>& paths) {

  const std::string key = "TextureAtlas" + std::to_string(asset_index);

  if (const auto it = assets_.find(key); it != assets_.end()) {
    return {it->second};
  }

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
