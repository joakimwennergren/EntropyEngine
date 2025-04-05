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

// CameraManager
#include "cameras/camera_manager.h"
#include "cameras/icamera_manger.h"

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
#include "vulkan/swapchains/swapchain.h"

// Surfaces
#include "vulkan/surfaces/surface.h"

// Textures
#include "vulkan/textures/depthbuffer_texture.h"
#include "vulkan/textures/swapchain_texture.h"
#include "vulkan/textures/texture.h"

// PipelineCache
#include "vulkan/pipelinecaches/ipipeline_cache.h"
#include "vulkan/pipelinecaches/pipeline_cache.h"

// DescriptorPools
#include "vulkan/descriptorpools/descriptorpool.h"
#include "vulkan/descriptorpools/idescriptorpool.h"

// CommandPool
#include "vulkan/commandpools/commandpool.h"
#include "vulkan/commandpools/icommandpool.h"

// Memory Allocator
#include "vulkan/memory/allocator.h"
#include "vulkan/memory/iallocator.h"

// Vulkan Devices
#include "vulkan/devices/ilogical_device.h"
#include "vulkan/devices/iphysical_device.h"
#include "vulkan/devices/logical_device.h"
#include "vulkan/devices/physical_device.h"

// Vulkan Instance
#include "vulkan/instances/ivk_instance.h"
#include "vulkan/instances/vk_instance.h"

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
using namespace Entropy::Vulkan::Renderers;
using namespace Entropy::ECS;
using namespace Entropy::Assets;
using namespace Entropy::Cameras;

void OnFramebufferResize(GLFWwindow* window, const int width,
                         const int height) {
  if (const auto renderer =
          static_cast<VulkanRenderer*>(glfwGetWindowUserPointer(window));
      renderer != nullptr) {
    renderer->Resize(width, height);
    //renderer->Render(width, height);
  }
}

flecs::entity CreateSprite(const std::string& path,
                           const glm::vec3 pos = glm::vec3(0.0f),
                           const glm::vec3 dim = glm::vec3(100.0f)) {

  const ServiceLocator* sl = ServiceLocator::GetInstance();
  const auto textureId =
      sl->getService<IAssetManager>()->LoadAsync<Texture>(path);
  return sl->getService<IWorld>()
      ->Get()
      ->entity()
      .set<Components::Position>({pos})
      .set<Components::Dimension>({dim})
      .set<Components::TwoDQuad>({})
      .set<Components::Texture>({path, textureId, true});
}

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
  sl->getService<IAssetManager>()->Unload<Texture>("test.png");
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

extern "C" void Entity_AddPosition(flecs::entity* entity, CPosition pos) {
  if (entity == nullptr) {
    std::cout << "Entity is null." << std::endl;
    return;
  }
  entity->set<Components::Position>({glm::vec3{pos.x, pos.y, pos.z}});
}

extern "C" void Entity_Add2DQuad(flecs::entity* entity) {
  if (entity == nullptr) {
    std::cout << "Entity is null." << std::endl;
    return;
  }
  entity->set<Components::TwoDQuad>({});
}

extern "C" void Entity_AddDimension(flecs::entity* entity, CPosition dim) {
  if (entity == nullptr) {
    std::cout << "Entity is null." << std::endl;
    return;
  }
  entity->set<Components::Dimension>({glm::vec3{dim.x, dim.y, dim.z}});
}

int main() {

  InitializeQuill();

  // Register vulkan services
  ServiceLocator* sl = ServiceLocator::GetInstance();
  sl->RegisterService(std::make_shared<VulkanInstance>());
  sl->RegisterService(std::make_shared<PhysicalDevice>());
  sl->RegisterService(std::make_shared<LogicalDevice>());
  sl->RegisterService(std::make_shared<Allocator>());
  sl->RegisterService(std::make_shared<CommandPool>());
  sl->RegisterService(std::make_shared<DescriptorPool>());
  sl->RegisterService(std::make_shared<PipelineCache>());
  sl->RegisterService(std::make_shared<SwapChain>());
  sl->RegisterService(std::make_shared<World>());
  sl->RegisterService(std::make_shared<AssetManager>());
  sl->RegisterService(std::make_shared<CameraManager>());

  const auto camera_manager = sl->getService<ICameraManager>();
  camera_manager->SetCurrentCamera(std::make_shared<OrthographicCamera>());

#if ENTROPY_ENABLE_SCRIPTING

  mono_set_dirs(MONO_LIBRARY_DIR_PATH, MONO_LIBRARY_ETC_PATH);
  mono_config_parse(MONO_LIBRARY_CONFIG);
  domain = mono_jit_init("test");

  // Bind the Texture class to C#
  mono_add_internal_call("Entropy.Texture::Internal_Create",
                         (void*)Texture_Create);
  mono_add_internal_call("Entropy.Texture::Internal_Destroy",
                         (void*)Texture_Destroy);

  // Bind components to C#
  mono_add_internal_call("Entropy.ECS.NativeBindings::Entity_AddPosition",
                         (void*)Entity_AddPosition);
  mono_add_internal_call("Entropy.ECS.NativeBindings::Entity_Add2DQuad",
                         (void*)Entity_Add2DQuad);
  mono_add_internal_call("Entropy.ECS.NativeBindings::Entity_AddDimension",
                         (void*)Entity_AddDimension);

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

  if (!glfwInit()) {
    LOG_INFO(logger_, "Could not initialize GLFW.");
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  GLFWwindow* window =
      glfwCreateWindow(640, 640, "Entropy application", nullptr, nullptr);

  if (!window) {
    LOG_INFO(logger_, "Could not create GLFW window.");
    glfwTerminate();
    return -1;
  }

  float xscale, yscale;
  glfwGetMonitorContentScale(glfwGetPrimaryMonitor(), &xscale, &yscale);

  const auto surface = std::make_shared<Surface>(window);
  const auto swapchain = sl->getService<ISwapChain>();

  swapchain->Build(surface,
                   VkExtent2D{static_cast<uint32_t>(640 * xscale),
                              static_cast<uint32_t>(640 * yscale)},
                   nullptr);

  const auto renderer = new VulkanRenderer(static_cast<uint32_t>(640 * xscale),
                                           static_cast<uint32_t>(640 * yscale));

  /*
  CreateSprite("test.png", glm::vec3{500, 500, 0.0f},
               glm::vec3{200, 200, 0.0f});
  CreateSprite("test2.png", glm::vec3{200, 200, 0.0f},
               glm::vec3{200, 200, 0.0f});
               */

  glfwSetWindowUserPointer(window, renderer);
  glfwSetFramebufferSizeCallback(window, OnFramebufferResize);

  while (!glfwWindowShouldClose(window)) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    renderer->Render(static_cast<uint32_t>(width * xscale),
                     static_cast<uint32_t>(height * yscale));
    glfwPollEvents();
  }
  // Cleanup
  glfwTerminate();

#if ENTROPY_ENABLE_SCRIPTING
  mono_jit_cleanup(domain);
#endif

  delete renderer;

  // Unregister services
  sl->UnregisterService<ICameraManager>();
  sl->UnregisterService<IAssetManager>();
  sl->UnregisterService<IWorld>();
  sl->UnregisterService<ISwapChain>();
  sl->UnregisterService<IPipelineCache>();
  sl->UnregisterService<IDescriptorPool>();
  sl->UnregisterService<ICommandPool>();
  sl->UnregisterService<IAllocator>();
  sl->UnregisterService<ILogicalDevice>();
  sl->UnregisterService<IPhysicalDevice>();
  sl->UnregisterService<IVulkanInstance>();

  return EXIT_SUCCESS;
}