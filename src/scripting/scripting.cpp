#include "scripting.h"

#include <vulkan/textures/textureatlas.h>

#include <iostream>

#include "ecs/components/asset.h"
#include "ecs/iworld.h"
#include "ecs/tags/2d.h"
#include "loggers/logger.h"
#include "servicelocators/servicelocator.h"

using namespace Entropy::ECS::Tags;
using namespace Entropy::ECS::Components;
using namespace Entropy::Assets;

flecs::entity* Entity_Create() {
  LOG_INFO(logger_, "Creating entity");
  const flecs::entity entity = ServiceLocator::GetInstance()
                                   ->getService<Entropy::ECS::IWorld>()
                                   ->Get()
                                   ->entity();
  entity.set<D2>({});  // @TODO make this more generic not only 2d..
  return new flecs::entity(entity);
}

void Entity_Destroy(const flecs::entity* entity) {
  LOG_INFO(logger_, "PRE Destroying entity");
  if (entity != nullptr && entity->is_valid()) {
    LOG_INFO(logger_, "Destroying entity");
    entity->destruct();
    delete entity;
  }
}

void Entity_AddPosition(const flecs::entity* entity, Position pos) {
  if (entity != nullptr) {
    (void)entity->set<Position>({pos});
  }
}

void Entity_AddDimension(const flecs::entity* entity, Dimension dim) {
  if (entity != nullptr) {
    (void)entity->set<Dimension>({dim});
  }
}

void Entity_AddRotation(const flecs::entity* entity, Rotation rot) {
  if (entity != nullptr) {
    (void)entity->set<Rotation>({rot});
  }
}

void Entity_AddAsset(const flecs::entity* entity,
                     IAssetManager::AssetHandle asset_handle) {
  if (entity != nullptr) {
    const auto asset_handles = std::vector{asset_handle};
    entity->set<Asset>({asset_handles});
  }
}

/*
IAssetManager::AssetHandle TextureAtlas_Create(MonoArray* paths) {
    std::vector<std::string> stringPaths;
    const uint32_t length = mono_array_length(paths);
    for (uint32_t i = 0; i < length; ++i) {
        auto* str = mono_array_get(paths, MonoString*, i);
        char* utf8 = mono_string_to_utf8(str);
        stringPaths.emplace_back(utf8);
        mono_free(utf8);
    }
    const ServiceLocator* sl = ServiceLocator::GetInstance();
    const auto asset_handles = sl->getService<IAssetManager>()->Load(stringPaths, IAssetManager::kLoadTextureAtlasSync);
    return asset_handles[0];
}

void TextureAtlas_AddAndRebuild(const IAssetManager::AssetHandle& handle, MonoString *path) {
  char* utf8 = mono_string_to_utf8(path);
  const auto atlas = static_cast<Entropy::Vulkan::Textures::TextureAtlas*>(handle.asset);
  atlas->image_paths.emplace_back(utf8);
  atlas->CreateAtlas();
  mono_free(utf8);
}
*/

void BindInternalCalls() {
  /*
    // Entity creation & destruction
    mono_add_internal_call("Entropy.ECS.Entity::Entity_Create",
      reinterpret_cast<void*>(Entity_Create));
    mono_add_internal_call("Entropy.ECS.Entity::Entity_Destroy",
      reinterpret_cast<void*>(Entity_Destroy));

    // Entity components
    mono_add_internal_call("Entropy.NativeBindings::Entity_AddPosition",
        reinterpret_cast<void*>(Entity_AddPosition));
    mono_add_internal_call("Entropy.NativeBindings::Entity_AddDimension",
        reinterpret_cast<void*>(Entity_AddDimension));
    mono_add_internal_call("Entropy.NativeBindings::Entity_AddRotation",
        reinterpret_cast<void*>(Entity_AddRotation));
    mono_add_internal_call("Entropy.NativeBindings::Entity_AddAsset",
        reinterpret_cast<void*>(Entity_AddAsset));

    // Texture atlas
    mono_add_internal_call("Entropy.TextureAtlas::TextureAtlas_Create",
      reinterpret_cast<void*>(TextureAtlas_Create));
      */
}
