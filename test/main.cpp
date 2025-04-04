#include <GLFW/glfw3.h>
#include <iostream>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-config.h>

// Renderers
#include "renderers/vulkan_renderer.h"

// sync
#include "vulkan/synchronization/synchronizer.h"

// Cameras
#include "cameras/orthographic_camera.h"

// CameraManager
#include "cameras/camera_manager.h"
#include "cameras/icamera_manger.h"

// TextureManager
#include "assets/iasset_manager.h"
#include "assets/asset_manager.h"

// ECS
#include "ecs/components/2d_quad.h"
#include "ecs/components/dimension.h"
#include "ecs/components/position.h"
#include "ecs/components/sprite.h"
#include "ecs/components/texture.h"
#include "ecs/iworld.h"
#include "ecs/world.h"

// Pipelines
#include "vulkan/pipelines/twod_pipeline.h"

// RenderPass
#include "vulkan/renderpasses/renderpass.h"

// SwapChain
#include "vulkan/swapchains/iswapchain.h"
#include "vulkan/swapchains/swapchain.h"

// Surfaces
#include "vulkan/surfaces/surface.h"

// Textures
#include "vulkan/textures/depthbuffer_texture.h"
#include "vulkan/textures/swapchain_texture.h"
#include "vulkan/textures/texture.h"

// Shader
#include "vulkan/shaders/shader.h"

// PipelineCache
#include "vulkan/pipelinecaches/ipipeline_cache.h"
#include "vulkan/pipelinecaches/pipeline_cache.h"

// DescriptorSet
#include "vulkan/descriptorsets/descriptorset.h"

// DescriptorSetLayout
#include "vulkan/descriptorsetlayouts/descriptorset_layout.h"

// DescriptorPools
#include "vulkan/descriptorpools/descriptorpool.h"
#include "vulkan/descriptorpools/idescriptorpool.h"

// CommandBuffer
#include "vulkan/commandbuffers/commandbuffer.h"

// CommandPool
#include "vulkan/commandpools/commandpool.h"
#include "vulkan/commandpools/icommandpool.h"

// Buffers
#include "vulkan/buffers/base_buffer.h"
#include "vulkan/buffers/index_buffer.h"
#include "vulkan/buffers/staging_buffer.h"
#include "vulkan/buffers/storage_buffer.h"
#include "vulkan/buffers/uniform_buffer.h"
#include "vulkan/buffers/vertex_buffer.h"

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

void OnFramebufferResize(GLFWwindow *window, const int width,
                         const int height) {
  if (const auto renderer =
          static_cast<VulkanRenderer *>(glfwGetWindowUserPointer(window));
      renderer != nullptr) {
    renderer->Resize(width, height);
    renderer->Render(width, height);
  }
}

flecs::entity CreateSprite(const std::string &path,
                  const glm::vec3 pos = glm::vec3(0.0f),
                  const glm::vec3 dim = glm::vec3(100.0f)) {

  const ServiceLocator *sl = ServiceLocator::GetInstance();
  const auto textureId = sl->getService<IAssetManager>()->LoadAsync<Texture>(path);
   return sl->getService<IWorld>()->Get()->entity()
        .set<Components::Position>({pos})
        .set<Components::Dimension>({dim})
        .set<Components::TwoDQuad>({})
        .set<Components::Texture>({path, textureId, true});
}

void Test() {

  if (!glfwInit()) {
    LOG_INFO(logger_, "Could not initialize GLFW.");
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  GLFWwindow *window =
      glfwCreateWindow(640, 640, "Entropy application", nullptr, nullptr);

  if (!window) {
    LOG_INFO(logger_, "Could not create GLFW window.");
    glfwTerminate();
    return;
  }

    float xscale, yscale;
    glfwGetMonitorContentScale(glfwGetPrimaryMonitor(), &xscale, &yscale);

  const auto surface = std::make_shared<Surface>(window);

  const ServiceLocator *sl = ServiceLocator::GetInstance();
  const auto swapchain = sl->getService<ISwapChain>();

  swapchain->Build(surface, VkExtent2D{static_cast<uint32_t>(640 * xscale), static_cast<uint32_t>(640 * yscale)}, nullptr);

  const auto batch_renderer = new VulkanRenderer(static_cast<uint32_t>(640 * xscale), static_cast<uint32_t>(640 * yscale));
    CreateSprite("test.png", glm::vec3{500, 500, 0.0f},
               glm::vec3{200, 200, 0.0f});
    CreateSprite("test2.png", glm::vec3{200, 200, 0.0f},
             glm::vec3{200, 200, 0.0f});

  glfwSetWindowUserPointer(window, batch_renderer);
  glfwSetFramebufferSizeCallback(window, OnFramebufferResize);

#if 1
  while (!glfwWindowShouldClose(window)) {
    batch_renderer->Render(static_cast<uint32_t>(640 * xscale), static_cast<uint32_t>(640 * yscale));
    glfwPollEvents();
  }
#endif
  delete batch_renderer;
  // Cleanup
  glfwTerminate();
}

MonoDomain *domain;
MonoAssembly *assembly;
MonoImage *image;

int main() {

      InitializeQuill();

  // Register vulkan services
  ServiceLocator *sl = ServiceLocator::GetInstance();
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

    mono_set_dirs(MONO_LIBRARY_DIR_PATH, MONO_LIBRARY_ETC_PATH);
    mono_config_parse (MONO_LIBRARY_CONFIG);
    domain = mono_jit_init ("test");

    // Load the GameScript assembly
    assembly = mono_domain_assembly_open(domain, "GameScripts.dll");
    if (!assembly) {
        std::cout  << "Failed to load GameScript.dll!" << std::endl;
    }

    // Get the Mono image from the assembly
    image = mono_assembly_get_image(assembly);

    // Get the class from the assembly (namespace and class name)
    MonoClass *klass = mono_class_from_name(image, "", "GameScript");
    if (!klass) {
        std::cerr << "Failed to find class GameScript!" << std::endl;
        //return;
    }

    // Get the method to invoke (method name and signature)
    MonoMethod *method = mono_class_get_method_from_name(klass, "Start", 0);  // No parameters
    if (!method) {
        std::cerr << "Failed to find method Start!" << std::endl;
        //return;
    }

    // Create an instance of the class
    MonoObject *object = mono_object_new(domain, klass);
    mono_runtime_object_init(object);

    // Invoke the method on the created object
    MonoObject *result = mono_runtime_invoke(method, object, nullptr, nullptr);

  Test();

    mono_jit_cleanup(domain);

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