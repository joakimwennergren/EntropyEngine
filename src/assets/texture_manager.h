#ifndef ENTROPY_ASSETS_TEXTURE_MANAGER_H
#define ENTROPY_ASSETS_TEXTURE_MANAGER_H

#include <flecs.h>
#include <string>
#include <unordered_map>
#include "itexture_manager.h"


namespace Entropy::Assets::Textures {

class TextureManager final : public ServiceBase<ITextureManager> {
public:
  flecs::entity LoadTexture(const std::string &path) override;
  void ReleaseTexture(flecs::entity texture_entity) override;
  flecs::entity GetTexture(const std::string &path) override;

private:
  int32_t next_texture_id_ = 0;
  std::unordered_map<std::string, flecs::entity>
    texture_map; // Maps paths to ECS entities
};

} // namespace Entropy::Assets::Textures

#endif // ENTROPY_ASSETS_TEXTURE_MANAGER_H
