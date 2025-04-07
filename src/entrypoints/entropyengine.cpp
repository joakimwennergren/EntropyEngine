#include "entropyengine.h"

// Cameras
#include "cameras/camera_manager.h"
#include "cameras/icamera_manger.h"
#include "cameras/orthographic_camera.h"

// AssetManager
#include "assets/asset_manager.h"
#include "assets/iasset_manager.h"

// ECS
#include "ecs/iworld.h"
#include "ecs/world.h"

// SwapChain
#include "vulkan/swapchains/iswapchain.h"
#include "vulkan/swapchains/swapchain.h"

// Surfaces
#include "vulkan/surfaces/surface.h"

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

using namespace Entropy::EntryPoints;
using namespace Entropy::Graphics::Vulkan::Instances;
using namespace Entropy::Graphics::Vulkan::Devices;
using namespace Entropy::Graphics::Vulkan::Memory;
using namespace Entropy::Graphics::Vulkan::CommandPools;
using namespace Entropy::Graphics::Vulkan::DescriptorPools;
using namespace Entropy::Graphics::Vulkan::Caches;
using namespace Entropy::Graphics::Vulkan::Surfaces;
using namespace Entropy::Graphics::Vulkan::SwapChains;
using namespace Entropy::Vulkan::Renderers;
using namespace Entropy::ECS;
using namespace Entropy::Assets;
using namespace Entropy::Cameras;

// @todo test!!
#include <ecs/components/2d_quad.h>
#include <ecs/components/dimension.h>
#include <ecs/components/position.h>
#include <ecs/components/texture.h>
#include <vulkan/textures/texture.h>
#include <glm/glm.hpp>

flecs::entity CreateSprite(const std::string& path,
                           const glm::vec3 pos = glm::vec3(0.0f),
                           const glm::vec3 dim = glm::vec3(500.0f)) {

  const ServiceLocator* sl = ServiceLocator::GetInstance();
  const auto textureId =
      sl->getService<IAssetManager>()
          ->LoadAsync<Entropy::Graphics::Vulkan::Textures::Texture>(path);

  return sl->getService<IWorld>()
      ->Get()
      ->entity()
      .set<Components::Position>({pos})
      .set<Components::Dimension>({dim})
      .set<Components::TwoDQuad>({})
      .set<Components::Texture>({path, textureId, true});
}

EntropyEngine::EntropyEngine(void* layer, uint32_t width, uint32_t height) {
  InitializeQuill();
  SetupServices();
  const auto sl = ServiceLocator::GetInstance();
  sl->getService<ISwapChain>()->Build(
      std::make_shared<Surface>((CAMetalLayer*)layer),
      VkExtent2D{width, height}, nullptr);
  const auto camera_manager = sl->getService<ICameraManager>();
  camera_manager->SetCurrentCamera(std::make_shared<OrthographicCamera>());
  renderer = new VulkanRenderer(width, height);
  CreateSprite("test.png");
}

/*
EntropyEngine::EntropyEngine() {
  InitializeQuill();
  SetupServices();
}
  */

EntropyEngine::~EntropyEngine() {
  delete renderer;
  TeardownServices();
}

void EntropyEngine::SetupServices() {
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
}

void EntropyEngine::TeardownServices() {
  // Unregister vulkan services
  ServiceLocator* sl = ServiceLocator::GetInstance();
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
}
