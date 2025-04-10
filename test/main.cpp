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
#include "ecs/components/2d_quad.h"
#include "ecs/components/dimension.h"
#include "ecs/components/position.h"
#include "ecs/components/texture.h"
#include "ecs/iworld.h"
#include "ecs/world.h"

// SwapChain
#include "vulkan/swapchains/iswapchain.h"

// Surfaces
#include "vulkan/surfaces/surface.h"

// Textures
#include "vulkan/textures/depthbuffer_texture.h"
#include "vulkan/textures/texture.h"

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
#include <ecs/tags/2d.h>
#include <entrypoints/entropyengine.h>

#include "vulkan/instances/ivk_instance.h"

#include "loggers/logger.h"
#include "servicelocators/servicelocator.h"

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

extern "C" int32_t Texture_Create(MonoString* path) {
  // Your texture creation logic

  const ServiceLocator* sl = ServiceLocator::GetInstance();
  const auto textureId = sl->getService<IAssetManager>()->LoadAsync<Texture>(
      mono_string_to_utf8(path));

  std::cout << "Texture created async with path: " << mono_string_to_utf8(path)
            << std::endl;

  return textureId;
}

extern "C" void Texture_Destroy(const int32_t textureId) {
  // Your texture destruction logic
  const ServiceLocator* sl = ServiceLocator::GetInstance();
  sl->getService<IAssetManager>()->Unload<Texture>(textureId);
}

extern "C" flecs::entity* Entity_Create() {
  std::cout << "Entity created." << std::endl;
  const ServiceLocator* sl = ServiceLocator::GetInstance();
  flecs::entity entity = sl->getService<IWorld>()->Get()->entity();
  return new flecs::entity(entity);
}

extern "C" void Entity_Destroy(const flecs::entity* entity) {
  if (entity != nullptr && entity->is_valid()) {
    std::cout << "Entity destroyed." << std::endl;
    entity->destruct();
    delete entity;
  } else {
    std::cout << "Entity is not valid." << std::endl;
    return;
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
  if (entity == nullptr) {
    std::cout << "Entity is null." << std::endl;
    return;
  }

  const ServiceLocator* sl = ServiceLocator::GetInstance();
  const auto textureId =
      sl->getService<IAssetManager>()->LoadToAtlas(mono_string_to_utf8(path));

  std::cout << "Texture created async with path: " << mono_string_to_utf8(path)
            << std::endl;

  entity->set<Components::Texture>(
      {mono_string_to_utf8(path), textureId, true});
}

int main() {
  const auto engine = Entropy::EntryPoints::EntropyEngine(820, 820);

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