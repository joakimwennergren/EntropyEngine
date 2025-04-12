#include <GLFW/glfw3.h>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-config.h>
#include <cstdint>
#include <iostream>

// Renderers
#include "renderers/vulkan_renderer.h"

// Cameras
#include "cameras/orthographic_camera.h"

// TextureManager
#include "assets/asset_manager.h"
#include "assets/iasset_manager.h"

// ECS
#include "ecs/components/dimension.h"
#include "ecs/components/position.h"
#include "ecs/iworld.h"
#include "ecs/world.h"

// SwapChain
#include "vulkan/swapchains/iswapchain.h"

// Surfaces
#include "vulkan/surfaces/surface.h"

// PipelineCache
#include "vulkan/pipelinecaches/ipipeline_cache.h"

// DescriptorPools
#include "vulkan/descriptorpools/idescriptorpool.h"

// CommandPool
#include "vulkan/commandpools/commandpool.h"

// Memory Allocator
#include "vulkan/memory/iallocator.h"

// Vulkan Devices
#include "vulkan/devices/ilogical_device.h"

// Vulkan Instance
#include <ecs/components/asset.h>
#include <ecs/tags/2d.h>
#include <ecs/tags/textureatlas.h>
#include <entrypoints/entropyengine.h>
#include <vulkan/textures/textureatlas.h>

#include "loggers/logger.h"
#include "servicelocators/servicelocator.h"
#include "vulkan/instances/ivk_instance.h"

#include "config.h"

using namespace Entropy::Graphics::Vulkan::Instances;
using namespace Entropy::Graphics::Vulkan::Devices;
using namespace Entropy::Graphics::Vulkan::Memory;
using namespace Entropy::Graphics::Vulkan::Buffers;
using namespace Entropy::Graphics::Vulkan::Data;
using namespace Entropy::Graphics::Vulkan::CommandPools;
using namespace Entropy::Graphics::Vulkan::CommandBuffers;
using namespace Entropy::Graphics::Vulkan::DescriptorPools;
using namespace Entropy::Graphics::Vulkan::DescriptorSetLayouts;
using namespace Entropy::Graphics::Vulkan::DescriptorSets;
using namespace Entropy::Graphics::Vulkan::Caches;
using namespace Entropy::Graphics::Vulkan::Shaders;
using namespace Entropy::Graphics::Vulkan::Textures;
using namespace Entropy::Graphics::Vulkan::Surfaces;
using namespace Entropy::Graphics::Vulkan::SwapChains;
using namespace Entropy::Graphics::Vulkan::RenderPasses;
using namespace Entropy::Graphics::Vulkan::Pipelines;
using namespace Entropy::Graphics::Vulkan::Synchronization;
using namespace Entropy::Renderers;
using namespace Entropy::ECS;
using namespace Entropy::Assets;
using namespace Entropy::Cameras;

MonoDomain* domain;
MonoAssembly* assembly;
MonoImage* image;

extern "C" void Texture_Create(MonoString* path) {
  const ServiceLocator* sl = ServiceLocator::GetInstance();
  std::vector<std::string> paths;
  paths.emplace_back(mono_string_to_utf8(path));
  const auto handle = sl->getService<IAssetManager>()->Load(paths, IAssetManager::kLoadTextureSync);
}

extern "C" void Texture_Destroy(const int32_t textureId) {
  // Your texture destruction logic
  const ServiceLocator* sl = ServiceLocator::GetInstance();
  //sl->getService<IAssetManager>()->Unload<Texture>(textureId);
}

extern "C" flecs::entity* Entity_Create() {
  std::cout << "Entity created." << std::endl;
  const ServiceLocator* sl = ServiceLocator::GetInstance();
  const flecs::entity entity = sl->getService<IWorld>()->Get()->entity();
  return new flecs::entity(entity);
}

extern "C" void Entity_Destroy(const flecs::entity* entity) {
  if (entity != nullptr && entity->is_valid()) {
    std::cout << "Entity destroyed." << std::endl;
    entity->destruct();
    delete entity;
  } else {
    std::cout << "Entity is not valid." << std::endl;
  }
}

extern "C" struct CPosition {
  float x, y, z;
};

extern "C" struct CDimension {
  float x, y, z;
};

extern "C" void Entity_AddPosition(const flecs::entity* entity, CPosition pos) {
  if (entity == nullptr) {
    std::cout << "Entity is null." << std::endl;
    return;
  }
  entity->set<Components::Position>({glm::vec3{pos.x, pos.y, pos.z}});
}

extern "C" void Entity_Add2DQuad(const flecs::entity* entity) {
  if (entity == nullptr) {
    std::cout << "Entity is null." << std::endl;
    return;
  }
  (void)entity->add<Tags::D2>();
}

extern "C" void Entity_AddDimension(const flecs::entity* entity, CPosition dim) {
  if (entity == nullptr) {
    std::cout << "Entity is null." << std::endl;
    return;
  }
  entity->set<Components::Dimension>({glm::vec3{dim.x, dim.y, dim.z}});
}

extern "C" void Entity_AddTexture(const flecs::entity* entity,
                                  MonoString* path) {
  if (entity != nullptr) {
    const ServiceLocator* sl = ServiceLocator::GetInstance();
    std::vector<std::string> paths;
    paths.emplace_back(mono_string_to_utf8(path));
    const auto asset_handles = sl->getService<IAssetManager>()->Load(paths, IAssetManager::kLoadTextureAtlasSync);
    entity->set<Components::Asset>({asset_handles});
    const auto num_textures = static_cast<TextureAtlas*>(asset_handles[0].asset)->imagePaths_.size() - 1;
    entity->set<Tags::TextureAtlas>({asset_handles[0].index, static_cast<int32_t>(num_textures)});
  }
}

extern "C" void Entity_AddTextureAtlas(const flecs::entity* entity,
                                  MonoArray* paths) {
  if (entity != nullptr) {
    std::vector<std::string> stringPaths;
    const uint32_t length = mono_array_length(paths);
    for (int i = 0; i < length; ++i) {
      auto* str = mono_array_get(paths, MonoString*, i);
      char* utf8 = mono_string_to_utf8(str);
      // Use utf8...
      stringPaths.emplace_back(utf8);
      mono_free(utf8);
    }
    const ServiceLocator* sl = ServiceLocator::GetInstance();
    const auto asset_handles = sl->getService<IAssetManager>()->Load(stringPaths, IAssetManager::kLoadTextureAtlasSync);
    entity->set<Components::Asset>({asset_handles});
    const auto num_textures = static_cast<TextureAtlas*>(asset_handles[0].asset)->imagePaths_.size() - 1;
    entity->set<Tags::TextureAtlas>({asset_handles[0].index, static_cast<int32_t>(num_textures)});
  }
}

int main() {
  const auto engine = Entropy::EntryPoints::EntropyEngine(1266, 585);

#if ENTROPY_ENABLE_SCRIPTING

  mono_set_dirs(MONO_LIBRARY_DIR_PATH, MONO_LIBRARY_ETC_PATH);
  mono_config_parse(MONO_LIBRARY_CONFIG);

  mono_set_assemblies_path(".");
  domain = mono_jit_init("test");

  // Bind the Texture class to C#
  mono_add_internal_call("Entropy.Texture::Texture_Create",
                         (void*)Texture_Create);
  mono_add_internal_call("Entropy.Texture::Texture_Destroy",
                         (void*)Texture_Destroy);

  // Bind components to C#
  mono_add_internal_call("Entropy.ECS.NativeBindings::Entity_AddPosition",
                         (void*)Entity_AddPosition);
  mono_add_internal_call("Entropy.ECS.NativeBindings::Entity_Add2DQuad",
                         (void*)Entity_Add2DQuad);
  mono_add_internal_call("Entropy.ECS.NativeBindings::Entity_AddDimension",
                         (void*)Entity_AddDimension);
  mono_add_internal_call("Entropy.ECS.NativeBindings::Entity_AddTexture",
                         (void*)Entity_AddTexture);
  mono_add_internal_call("Entropy.ECS.NativeBindings::Entity_AddTextureAtlas",
                       (void*)Entity_AddTextureAtlas);

  // Bind the Entity class to C#
  mono_add_internal_call("Entropy.ECS.Entity::Entity_Create",
                         (void*)Entity_Create);
  mono_add_internal_call("Entropy.ECS.Entity::Entity_Destroy",
                         (void*)Entity_Destroy);

  // Load the GameScript assembly
  assembly = mono_domain_assembly_open(domain, "GameScripts.dll");
  if (!assembly) {
    std::cout << "Failed to load GameScript.dll!" << std::endl;
  }

  std::cout << "GameScript.dll loaded!" << std::endl;

  // Get the Mono image from the assembly
  image = mono_assembly_get_image(assembly);

  MonoClass* klass = mono_class_from_name(image, "", "EntityScript");
  MonoObject* obj = mono_object_new(domain, klass);
  mono_runtime_object_init(obj);

  MonoMethod* start = mono_class_get_method_from_name(klass, "Start", 0);
  MonoMethod* update = mono_class_get_method_from_name(klass, "Update", 0);

  if (start)
    mono_runtime_invoke(start, obj, nullptr, nullptr);
  if (update)
    mono_runtime_invoke(update, obj, nullptr, nullptr);

#endif

  engine.Run();


#if ENTROPY_ENABLE_SCRIPTING
  mono_jit_cleanup(domain);
#endif

  return EXIT_SUCCESS;
}