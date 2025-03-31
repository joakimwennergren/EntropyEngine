#include "texture_manager.h"
#include "ecs/components/texture.h"
#include "ecs/iworld.h"
#include "vulkan/textures/texture.h"

using namespace Entropy::Graphics::Vulkan::Textures;
using namespace Entropy::Assets::Textures;
using namespace Entropy::ECS;

flecs::entity TextureManager::LoadTexture(const std::string &path) {

  const ServiceLocator *sl = ServiceLocator::GetInstance();
  const auto world = sl->getService<IWorld>();

  // Check if the texture is already loaded
  if (const auto it = texture_map.find(path); it != texture_map.end()) {
    return it->second;
  }

  // Create Vulkan texture
  const auto texture = std::make_shared<Texture>(path);

  // Create an ECS entity for the texture
  const auto texture_entity = world->Get()->entity();
  texture_entity.set<Components::Texture>({next_texture_id_, path, texture});

  // Store the entity in the map
  texture_map[path] = texture_entity;

  next_texture_id_++;
  return texture_entity;
}

void TextureManager::ReleaseTexture(const flecs::entity texture_entity) {

  if (const auto texture = texture_entity.get<Components::Texture>()->texture;
      !texture)
    return;

  for (auto it = texture_map.begin(); it != texture_map.end(); ++it) {
    if (it->second == texture_entity) {
      texture_map.erase(it);
      break;
    }
  }

  // Remove the ECS entity
  texture_entity.destruct();
}

flecs::entity TextureManager::GetTexture(const std::string &path) {
  if (texture_map.find(path) != texture_map.end()) {
    return texture_map[path];
  }
  return {};
}