#include "world.h"

using namespace Entropy::ECS;

World::World() { gameWorld_ = std::make_shared<flecs::world>(); }