#ifndef ENTROPY_ASSETS_TEXTURE_MANAGER_H
#define ENTROPY_ASSETS_TEXTURE_MANAGER_H

#include <future>
#include <mutex>
#include <unordered_map>
#include "iasset_manager.h"

namespace Entropy::Assets {

class AssetManager final : public ServiceBase<IAssetManager> {
 protected:
  int32_t LoadTexture(const std::string& path) override;

  std::shared_ptr<Graphics::Vulkan::Textures::Texture> GetTexture(
      int32_t textureId) override;

  int32_t LoadTextureAsync(const std::string& path) override;
  std::shared_ptr<Graphics::Vulkan::Textures::Texture> GetTextureAsync(
    int32_t textureId) override;

  void UnloadTexture(int32_t textureId) override;

  std::unordered_map<
      std::string,
      std::pair<int32_t, std::shared_future<std::shared_ptr<Graphics::Vulkan::Textures::Texture>>>>
      textureFutures_;

  std::unordered_map<std::string,
          std::pair<int32_t, std::shared_ptr<Graphics::Vulkan::Textures::Texture>>>
      textures_;

private:
  std::string GetTextureKeyById(int32_t textureId);
  int32_t textureIndex_ = -1;
  std::mutex mutex_;
};

}  // namespace Entropy::Assets

#endif  // ENTROPY_ASSETS_TEXTURE_MANAGER_H
