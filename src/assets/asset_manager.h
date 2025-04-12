#ifndef ENTROPY_ASSETS_TEXTURE_MANAGER_H
#define ENTROPY_ASSETS_TEXTURE_MANAGER_H

#include <future>
#include <mutex>
#include <unordered_map>
#include "iasset_manager.h"

namespace Entropy::Assets {
class AssetManager final : public ServiceBase<IAssetManager> {
public:
 ~AssetManager() override;
 protected:
  AssetHandle LoadTexture(const std::string& path) override;
  AssetHandle LoadTextureAsync(const std::string& path) override;
  AssetHandle LoadTextureAtlas(const std::string& path) override;
  AssetHandle LoadTextureAtlasAsync(const std::string& path) override;
private:
  std::mutex mutex_;
  std::unordered_map<std::string, std::shared_future<AssetHandle>> futures_;
  std::unordered_map<std::string, AssetHandle> assets_;
  int32_t asset_index = 0;
  std::vector<std::string> loadedTexturesAtlas_;
};
}  // namespace Entropy::Assets
#endif  // ENTROPY_ASSETS_TEXTURE_MANAGER_H
