// Copyright 2025 Joakim Wennergren
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef ENTROPY_SCRIPTING_SCRIPTING_H
#define ENTROPY_SCRIPTING_SCRIPTING_H

#include <flecs.h>
#include "assets/iasset_manager.h"
#include "ecs/components/dimension.h"
#include "ecs/components/position.h"
#include "ecs/components/rotation.h"

extern "C" {
// Entity creation & destruction
uint64_t Entity_Create();
void Entity_Destroy(const uint64_t entity_id);

// Adding components
void Entity_AddPosition(const uint64_t entity_id,
                        Entropy::ECS::Components::Position pos);
void Entity_AddDimension(const uint64_t entity_id,
                         Entropy::ECS::Components::Dimension dim);
void Entity_AddRotation(const uint64_t entity_id,
                        Entropy::ECS::Components::Rotation rot);
//void Entity_AddAsset(const uint64_t entity_id,
//                     Entropy::Assets::IAssetManager::AssetHandle asset_handle);

// TextureAtlas
//Entropy::Assets::IAssetManager::AssetHandle TextureAtlas_Create(MonoArray* paths);
//void TextureAtlas_AddAndRebuild(const Entropy::Assets::IAssetManager::AssetHandle& handle, MonoString *path);
}

/*
extern "C" void Texture_Create(MonoString* path) {
  const ServiceLocator* sl = ServiceLocator::GetInstance();
  std::vector<std::string> paths;
  paths.emplace_back(mono_string_to_utf8(path));
  const auto handle = sl->GetService<IAssetManager>()->Load(paths, IAssetManager::kLoadTextureSync);
}

extern "C" void Texture_Destroy(const int32_t textureId) {
  // Your texture destruction logic
  const ServiceLocator* sl = ServiceLocator::GetInstance();
  //sl->GetService<IAssetManager>()->Unload<Texture>(textureId);
}
*/

/*
extern "C" void Entity_AddPosition(const flecs::entity* entity, CPosition pos) {
  if (entity == nullptr) {
    std::cout << "Entity is null." << std::endl;
    return;
  }
  entity->set<Components::Position>({glm::vec3{pos.x, pos.y, pos.z}});
}
  */

/*
extern "C" void Entity_AddDimension(const flecs::entity* entity,
                                    CPosition dim) {
  if (entity == nullptr) {
    std::cout << "Entity is null." << std::endl;
    return;
  }
  entity->set<Components::Dimension>({glm::vec3{dim.x, dim.y, dim.z}});
}
  */

/*
extern "C" void Entity_AddTexture(const flecs::entity* entity,
                                  MonoString* path) {
  if (entity != nullptr) {
    const ServiceLocator* sl = ServiceLocator::GetInstance();
    std::vector<std::string> paths;
    paths.emplace_back(mono_string_to_utf8(path));
    const auto asset_handles = sl->GetService<IAssetManager>()->Load(
        paths, IAssetManager::kLoadTextureAtlasSync);
    entity->set<Components::Asset>({asset_handles});
    const auto num_textures =
        static_cast<TextureAtlas*>(asset_handles[0].asset)->imagePaths_.size() -
        1;
    entity->set<Tags::TextureAtlas>(
        {asset_handles[0].index, static_cast<int32_t>(num_textures)});
  }
}
*/

/*
extern "C" {
enum AssetTypeC {
  kTexture,
  kTextureAtlas,
  kUnknown,
};
// C-compatible version of AssetHandle
struct AssetHandleC {
  void* asset;
  int32_t index;
  const char* path;  // C-style string!
  bool is_async;
  int32_t type;  // or enum as int
};
AssetHandleC TextureAtlas_Create(MonoArray* paths) {
  std::vector<std::string> stringPaths;
  const uint32_t length = mono_array_length(paths);
  for (uint32_t i = 0; i < length; ++i) {
    auto* str = mono_array_get(paths, MonoString*, i);
    char* utf8 = mono_string_to_utf8(str);
    stringPaths.emplace_back(utf8);
    mono_free(utf8);
  }

  const ServiceLocator* sl = ServiceLocator::GetInstance();
  const auto asset_handles_vec = sl->GetService<Entropy::Assets::IAssetManager>()->Load(
      stringPaths, Entropy::Assets::IAssetManager::kLoadTextureAtlasSync);
  // Allocate memory on heap
  return AssetHandleC{asset_handles_vec[0].asset, asset_handles_vec[0].index,
                      strdup(asset_handles_vec[0].path.c_str()), false,
                      kTextureAtlas};
}

void Entity_AddAsset(const flecs::entity* entity, AssetHandleC handle) {
  if (entity == nullptr) {
    std::cout << "Entity is null." << std::endl;
    return;
  }
  auto assetHandle = std::vector<Entropy::Assets::IAssetManager::AssetHandle>();
  assetHandle.push_back(Entropy::Assets::IAssetManager::AssetHandle{
      handle.asset, handle.index, handle.path, handle.is_async,
      static_cast<Entropy::Assets::IAssetManager::AssetType>(handle.type)});
  entity->set<Entropy::ECS::Components::Asset>({assetHandle});
}
void Entity_DestroyAsset(const AssetHandleC& handle) {
  free((void*)handle.path);
}
}
*/

#endif  // ENTROPY_SCRIPTING_SCRIPTING_H
