#include <gtest/gtest.h>

#include "assets/asset_manager.h"
#include "assets/iasset_manager.h"
#include "assets/texture_asset.h"
#include "loggers/logger.h"
#include "servicelocators/servicelocator.h"

using namespace Entropy::Assets;

class AssetManagerTest : public ::testing::Test {
 protected:
  void SetUp() override {
    InitializeQuill();
    ServiceLocator* sl = ServiceLocator::GetInstance();
    sl->RegisterService(std::make_shared<AssetManager>());
  }
  void TearDown() override {
    ServiceLocator* sl = ServiceLocator::GetInstance();
    sl->UnregisterService<IAssetManager>();
  }
};

TEST_F(AssetManagerTest, LoadTexture) {
  /*
  auto assetManager =
      ServiceLocator::GetInstance()->GetService<IAssetManager>();

  assetManager->RegisterLoader<TextureAsset>(
      [](const std::string& path) -> std::shared_ptr<TextureAsset> {
        // Replace with real texture loading code
        std::cout << "Loading texture from " << path << std::endl;
        return std::make_shared<TextureAsset>(path);
      });

  auto tex =
      assetManager->Load<TextureAsset>("diffuse", "textures/diffuse.png");
  std::cout << "Loaded: " << tex->GetName() << std::endl;

  auto futureTex = assetManager->LoadAsync<TextureAsset>(
      "specular", "textures/specular.png");

  auto asyncTex = futureTex.get();
  std::cout << "Async loaded: " << asyncTex->GetName() << std::endl;
  */
}