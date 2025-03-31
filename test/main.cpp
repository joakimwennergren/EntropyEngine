#include <GLFW/glfw3.h>
#include "entropy_test.h"
#include <ecs/components/sprite.h>
#include <cameras/orthographic_camera.h>
#include <renderers/vulkan_batch_renderer.h>

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
using namespace Entropy::Assets::Textures;
using namespace Entropy::Cameras;

void OnFramebufferResize(GLFWwindow *window, const int width,
                         const int height) {
  if (const auto renderer =
          static_cast<BatchRenderer *>(glfwGetWindowUserPointer(window));
      renderer != nullptr) {
    renderer->Resize(width, height);
    renderer->Render(width, height);
  }
}

void CreateSprite(const std::string &path,
                  const glm::vec3 pos = glm::vec3(0.0f),
                  const glm::vec3 dim = glm::vec3(100.0f)) {
  const ServiceLocator *sl = ServiceLocator::GetInstance();
  const auto texture_e = sl->getService<ITextureManager>()->LoadTexture(path);

  const auto quad_e = sl->getService<IWorld>()->Get()->entity();
  quad_e.set<Components::TwoDQuad>({});

  const auto pos_e = sl->getService<IWorld>()->Get()->entity();
  pos_e.set<Components::Position>({pos});

  const auto dimension_e = sl->getService<IWorld>()->Get()->entity();
  pos_e.set<Components::Dimension>({dim});

  const auto sprite_e = sl->getService<IWorld>()->Get()->entity();
  sprite_e.set<Components::Sprite>({quad_e, texture_e, pos_e, dimension_e,
                                    glm::vec4{0.0f, 0.0f, 1.0f, 1.0f}});
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

  const auto surface = std::make_shared<Surface>(window);

  const ServiceLocator *sl = ServiceLocator::GetInstance();
  const auto swapchain = sl->getService<ISwapChain>();

  swapchain->Build(surface, VkExtent2D{640, 640}, nullptr);

  const auto batch_renderer = new BatchRenderer();
  CreateSprite("test2.png", glm::vec3{0, 0, 0.0f});
  CreateSprite("test.png", glm::vec3{500, 500, 0.0f},
               glm::vec3{100, 100, 0.0f});
  CreateSprite("test2.png", glm::vec3{500.0f, 100.0f, 0.0f});

  glfwSetWindowUserPointer(window, batch_renderer);
  glfwSetFramebufferSizeCallback(window, OnFramebufferResize);

#if 1
  while (!glfwWindowShouldClose(window)) {
    // Poll window events
    batch_renderer->Render(640, 640);
    glfwPollEvents();
  }
#endif
  delete batch_renderer;
  // Cleanup
  glfwTerminate();
}

int main() {
  // Initialize quill, logging library
  InitializeQuill();

  // Register vulkan services
  ServiceLocator *sl = ServiceLocator::GetInstance();
  sl->registerService(std::make_shared<VulkanInstance>());
  sl->registerService(std::make_shared<PhysicalDevice>());
  sl->registerService(std::make_shared<LogicalDevice>());
  sl->registerService(std::make_shared<Allocator>());
  sl->registerService(std::make_shared<CommandPool>());
  sl->registerService(std::make_shared<DescriptorPool>());
  sl->registerService(std::make_shared<PipelineCache>());
  sl->registerService(std::make_shared<SwapChain>());
  sl->registerService(std::make_shared<World>());
  sl->registerService(std::make_shared<TextureManager>());
  sl->registerService(std::make_shared<CameraManager>());

  /*

  const auto vk_instance = sl->getService<IVulkanInstance>();
  const auto physical_device = sl->getService<IPhysicalDevice>();
  const auto logical_device = sl->getService<ILogicalDevice>();
  const auto allocator = sl->getService<IAllocator>();
  const auto command_pool = sl->getService<ICommandPool>();
  const auto descriptor_pool = sl->getService<IDescriptorPool>();
  const auto pipeline_cache = sl->getService<IPipelineCache>();

  const std::vector<uint32_t> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  auto index_buffer = IndexBuffer(data);

  const uint8_t uint8_data[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  auto staging_buffer = StagingBuffer(sizeof(uint8_data), uint8_data,
                                      VK_BUFFER_USAGE_TRANSFER_SRC_BIT);

  const std::vector<ThreeDAnimVertex> vertices = {{{-1.0f, -1.0f, 0.0f},
                                                   {1.0f, 1.0f, 1.0f},
                                                   {1.0f, 0.0f, 0.0f, 1.0f},
                                                   {0.0f, 0.0f},
                                                   {1.0f, 0.0f, 0.0f, 1.0f},
                                                   {1.0f, 0.0f, 0.0f, 1.0f}},
                                                  {{-1.0f, -1.0f, 0.0f},
                                                   {1.0f, 1.0f, 1.0f},
                                                   {1.0f, 0.0f, 0.0f, 1.0f},
                                                   {0.0f, 0.0f},
                                                   {1.0f, 0.0f, 0.0f, 1.0f},
                                                   {1.0f, 0.0f, 0.0f, 1.0f}},
                                                  {{-1.0f, -1.0f, 0.0f},
                                                   {1.0f, 1.0f, 1.0f},
                                                   {1.0f, 0.0f, 0.0f, 1.0f},
                                                   {0.0f, 0.0f},
                                                   {1.0f, 0.0f, 0.0f, 1.0f},
                                                   {1.0f, 0.0f, 0.0f, 1.0f}},
                                                  {{-1.0f, -1.0f, 0.0f},
                                                   {1.0f, 1.0f, 1.0f},
                                                   {1.0f, 0.0f, 0.0f, 1.0f},
                                                   {0.0f, 0.0f},
                                                   {1.0f, 0.0f, 0.0f, 1.0f},
                                                   {1.0f, 0.0f, 0.0f, 1.0f}}};
  auto vertex_buffer = VertexBuffer(vertices);

  auto storage_buffer = StorageBuffer(100);
  auto uniform_buffer = UniformBuffer(100);

  auto command_buf = CommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

  VkDescriptorSetLayoutBinding instanceLayoutBinding = {};
  instanceLayoutBinding.binding = 0;
  instanceLayoutBinding.descriptorType =
      VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  instanceLayoutBinding.descriptorCount = 1;
  instanceLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
  instanceLayoutBinding.pImmutableSamplers = nullptr;
  const std::vector bindings = {instanceLayoutBinding};
  const std::vector<VkDescriptorBindingFlags> bindingFlags = {};

  auto descriptor_set_layout =
      std::make_shared<DescriptorSetLayout>(bindings, bindingFlags);

  std::vector<std::shared_ptr<DescriptorSet>> descriptor_sets;
  for (uint32_t i = 0; i < MAX_DESCRIPTOR_SETS - (10); i++) {
    descriptor_sets.push_back(
        std::make_shared<DescriptorSet>(descriptor_set_layout));
  }

  auto shader = Shader("2d_shader_vert.spv", "2d_shader_frag.spv");

  auto texture = Texture("test.png");
  auto depth_texture = DepthBufferTexture(640, 640);
  auto swapchain_texture = SwapChainTexture(640, 640);

  const auto texture_manager = sl->getService<ITextureManager>();
  auto tex_e = texture_manager->LoadTexture("test.png");
  texture_manager->ReleaseTexture(tex_e);

  auto render_pass = std::make_shared<RenderPass>();
  render_pass->RecreateDepthBuffer(640, 640);
  render_pass->CreateFrameBuffers(640, 640);

  auto two_d_pipeline = TwoDPipeline(render_pass);

  auto sync = Synchronizer(3);

   */

  const auto camera_manager = sl->getService<ICameraManager>();
  camera_manager->SetCurrentCamera(std::make_shared<OrthographicCamera>());

  Test();

  // Unregister services
  sl->unregisterService<ICameraManager>();
  sl->unregisterService<ITextureManager>();
  sl->unregisterService<IWorld>();
  sl->unregisterService<ISwapChain>();
  sl->unregisterService<IPipelineCache>();
  sl->unregisterService<IDescriptorPool>();
  sl->unregisterService<ICommandPool>();
  sl->unregisterService<IAllocator>();
  sl->unregisterService<ILogicalDevice>();
  sl->unregisterService<IPhysicalDevice>();
  sl->unregisterService<IVulkanInstance>();

  return EXIT_SUCCESS;
}