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
using namespace Entropy::Vulkan::Instances;
using namespace Entropy::Vulkan::Devices;
using namespace Entropy::Vulkan::Memory;
using namespace Entropy::Vulkan::CommandPools;
using namespace Entropy::Vulkan::DescriptorPools;
using namespace Entropy::Vulkan::Caches;
using namespace Entropy::Vulkan::Surfaces;
using namespace Entropy::Vulkan::SwapChains;
using namespace Entropy::Renderers;
using namespace Entropy::ECS;
using namespace Entropy::Assets;
using namespace Entropy::Cameras;

#if ENTROPY_PLATFORM == IOS
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_metal.h>
EntropyEngine::EntropyEngine(void* layer, uint32_t width, uint32_t height) {
  InitializeQuill();
  RegisterServices();
  const auto sl = ServiceLocator::GetInstance();
  sl->GetService<ISwapChain>()->Build(
      std::make_shared<Surface>((CAMetalLayer*)layer),
      VkExtent2D{width, height}, nullptr);
  const auto camera_manager = sl->GetService<ICameraManager>();
  camera_manager->SetCurrentCamera(std::make_shared<OrthographicCamera>());
  sl->RegisterService<IRenderer>(
      std::make_shared<VulkanRenderer>(width, height));
  renderer = sl->GetService<IRenderer>();
}
void EntropyEngine::Run() const {}
#elif ENTROPY_PLATFORM == MACOS || ENTROPY_PLATFORM == LINUX
void EntropyEngine::OnFramebufferResize(GLFWwindow* window, const int width,
                                        const int height) {
  const auto sl = ServiceLocator::GetInstance();
  const auto renderer = sl->GetService<IRenderer>();
  const auto world = sl->GetService<IWorld>();
  renderer->Resize(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
  (void)world->Get()->progress();
  renderer->Render(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
}
EntropyEngine::EntropyEngine(const uint32_t width, const uint32_t height) {
  if (!glfwInit()) {
    LOG_ERROR(logger_, "Could not initialize GLFW.");
    return;
  }
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  window_ =
      glfwCreateWindow(width, height, "Entropy application", nullptr, nullptr);
  if (!window_) {
    LOG_ERROR(logger_, "Could not create GLFW window.");
    glfwTerminate();
    return;
  }
  glfwSetFramebufferSizeCallback(window_, OnFramebufferResize);
  float xscale, yscale;
  glfwGetMonitorContentScale(glfwGetPrimaryMonitor(), &xscale, &yscale);
  InitializeQuill();
  RegisterServices();
  const auto sl = ServiceLocator::GetInstance();
  sl->GetService<ISwapChain>()->Build(
      std::make_shared<Surface>(window_),
      VkExtent2D{static_cast<uint32_t>(width * xscale),
                 static_cast<uint32_t>(height * yscale)},
      nullptr);
  const auto camera_manager = sl->GetService<ICameraManager>();
  camera_manager->SetCurrentCamera(std::make_shared<OrthographicCamera>());
  sl->RegisterService<IRenderer>(
      std::make_shared<VulkanRenderer>(static_cast<uint32_t>(width * xscale),
                                       static_cast<uint32_t>(height * yscale)));
  sl->RegisterService(std::make_shared<World>());
}
void EntropyEngine::Run() const {
  const auto sl = ServiceLocator::GetInstance();
  const auto renderer = sl->GetService<IRenderer>();
  const auto world = sl->GetService<IWorld>();
  while (!glfwWindowShouldClose(window_)) {
    int width, height;
    glfwGetFramebufferSize(window_, &width, &height);
    // @TODO handle bool return
    (void)world->Get()->progress();
    renderer->Render(static_cast<uint32_t>(width),
                     static_cast<uint32_t>(height));
    glfwPollEvents();
  }
}
#endif

EntropyEngine::~EntropyEngine() {
  UnRegisterServices();
#if ENTROPY_PLATFORM == MACOS || ENTROPY_PLATFORM == LINUX
  glfwTerminate();
#endif
}

void EntropyEngine::RegisterServices() {
  ServiceLocator* sl = ServiceLocator::GetInstance();
  sl->RegisterService(std::make_shared<VulkanInstance>());
  sl->RegisterService(std::make_shared<PhysicalDevice>());
  sl->RegisterService(std::make_shared<LogicalDevice>());
  sl->RegisterService(std::make_shared<Allocator>());
  sl->RegisterService(std::make_shared<CommandPool>());
  sl->RegisterService(std::make_shared<DescriptorPool>());
  sl->RegisterService(std::make_shared<PipelineCache>());
  sl->RegisterService(std::make_shared<SwapChain>());
  sl->RegisterService(std::make_shared<AssetManager>());
  sl->RegisterService(std::make_shared<CameraManager>());
}

void EntropyEngine::UnRegisterServices() {
  ServiceLocator* sl = ServiceLocator::GetInstance();
  sl->UnregisterService<IRenderer>();
  sl->UnregisterService<IWorld>();
  sl->UnregisterService<ICameraManager>();
  sl->UnregisterService<IAssetManager>();
  sl->UnregisterService<ISwapChain>();
  sl->UnregisterService<IPipelineCache>();
  sl->UnregisterService<IDescriptorPool>();
  sl->UnregisterService<ICommandPool>();
  sl->UnregisterService<IAllocator>();
  sl->UnregisterService<ILogicalDevice>();
  sl->UnregisterService<IPhysicalDevice>();
  sl->UnregisterService<IVulkanInstance>();
}
