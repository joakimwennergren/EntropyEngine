#ifndef ENTROPY_ASSETS_TEXTURE_MANAGER_H
#define ENTROPY_ASSETS_TEXTURE_MANAGER_H

#include <cstdint>
#include <future>
#include <mutex>
#include <string>
#include <unordered_map>
#include "iasset_manager.h"

namespace Entropy::Assets {

class AssetManager final : public ServiceBase<IAssetManager> {
 protected:
  int32_t LoadTexture(const std::string& path) override;

  std::shared_ptr<Graphics::Vulkan::Textures::Texture> GetTexture(
      const int32_t textureId) override;

  int32_t LoadTextureAsync(const std::string& path) override;
  std::shared_ptr<Graphics::Vulkan::Textures::Texture> GetTextureAsync(
      const int32_t textureId) override;

  void UnloadTexture(const int32_t textureId) override;

  std::unordered_map<
      int32_t,
      std::shared_future<std::shared_ptr<Graphics::Vulkan::Textures::Texture>>>
      textureFutures_;
  std::unordered_map<int32_t,
                     std::shared_ptr<Graphics::Vulkan::Textures::Texture>>
      textures_;
  int32_t textureIndex_ = -1;
  std::mutex mutex_;
};

}  // namespace Entropy::Assets

#endif  // ENTROPY_ASSETS_TEXTURE_MANAGER_H
