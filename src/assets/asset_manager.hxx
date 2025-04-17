#ifndef ENTROPY_ASSETS_TEXTURE_MANAGER_H
#define ENTROPY_ASSETS_TEXTURE_MANAGER_H

/**
 * @TODO Save atlas to disk with regions in some format..
 * @TODO LOAD atlas from disk
*/
#include <future>
#include <mutex>
#include <unordered_map>
#include "iasset_manager.h"

namespace Entropy::Assets {
class AssetManager final : public ServiceBase<IAssetManager> {
public:
 ~AssetManager() override;
 protected:
  std::vector<AssetHandle> LoadTexture(const std::vector<std::string> &paths) override;
  std::vector<AssetHandle> LoadTextureAsync(const std::vector<std::string> &paths) override;
  std::vector<AssetHandle> LoadTextureAtlas(const std::vector<std::string> &paths) override;
  std::vector<AssetHandle> LoadTextureAtlasAsync(const std::vector<std::string> &paths) override;
private:
  std::mutex mutex_;
  std::unordered_map<std::string, std::shared_future<AssetHandle>> futures_;
  std::unordered_map<std::string, AssetHandle> assets_;
  int32_t asset_index = 0;
};
}  // namespace Entropy::Assets
#endif  // ENTROPY_ASSETS_TEXTURE_MANAGER_H
