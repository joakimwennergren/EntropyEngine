#include <GLFW/glfw3.h>
// #include <mono/jit/jit.h>
// #include <mono/metadata/assembly.h>
// #include <mono/metadata/mono-config.h>
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
#include "vulkan/instances/ivk_instance.h"
#include "scripting/scripting.h"
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

//MonoDomain* domain;
//MonoAssembly* assembly;
//MonoImage* image;

int main() {
  const auto engine = Entropy::EntryPoints::EntropyEngine(1266, 585);

#if 0

  mono_set_dirs(MONO_LIBRARY_DIR_PATH, MONO_LIBRARY_ETC_PATH);
  mono_config_parse(MONO_LIBRARY_CONFIG);

  mono_set_assemblies_path(".");
  domain = mono_jit_init("test");

  BindInternalCalls();

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
  // mono_jit_cleanup(domain);
#endif

  return EXIT_SUCCESS;
}