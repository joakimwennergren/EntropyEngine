#include "asset_manager.h"

using namespace Entropy::Assets;
using namespace Entropy::Graphics::Vulkan::Textures;

int32_t AssetManager::LoadTexture(const std::string& path) {
    LoadAsset(textures_, path);
    return textureIndex_++;
}

std::shared_ptr<Texture> AssetManager::GetTexture(const std::string& path) {
    return GetAsset(textures_, path);
}

int32_t AssetManager::LoadTextureAsync(const std::string& path) {
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
    textureIndex_++;

    return -1;
}

std::shared_ptr<Texture> AssetManager::GetTextureAsync(const std::string& path) {
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

void AssetManager::UnloadTexture(const std::string& path) {
    textures_.erase(path);
}
