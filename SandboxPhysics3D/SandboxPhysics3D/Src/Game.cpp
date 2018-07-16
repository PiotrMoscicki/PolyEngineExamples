#include "Game.hpp"

#include <ECS/DeferredTaskSystem.hpp>

#include "GameManager.hpp"
#include "GameManagerWorldComponent.hpp"

DEFINE_GAME(BulletTest)

using namespace Poly;
using namespace BT;


void BulletTest::Init()
{
	gEngine->RegisterGameUpdatePhase(GameManagerSystem::Update);

	Scene* world = gEngine->GetActiveScene();
	DeferredTaskSystem::AddWorldComponentImmediate<BT::GameManagerWorldComponent>(world);

	GameManagerSystem::InitializeDemoWorld(world);
};

void BulletTest::Deinit()
{
}