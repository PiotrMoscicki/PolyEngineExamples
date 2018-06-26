#include "GameDebugSystem.hpp"

#include <ECS/World.hpp>
#include "GameDebugWorldComponent.hpp"
#include <ECS/DeferredTaskSystem.hpp>
#include <UI/ScreenSpaceTextComponent.hpp>
#include "GameManagerWorldComponent.hpp"
#include <Physics2D/Rigidbody2DComponent.hpp>

using namespace Poly;

void SGJ::Debug::DebugSystemPhase(Poly::World* world)
{
	GameDebugWorldComponent* com = world->GetWorldComponent<GameDebugWorldComponent>();

	Poly::ScreenSize size = gEngine->GetRenderingDevice()->GetScreenSize();


	if (!com->TextID)
	{
		com->TextID = DeferredTaskSystem::SpawnEntityImmediate(world);

		DeferredTaskSystem::AddComponentImmediate<ScreenSpaceTextComponent>(world, com->TextID, Vector2i(size.Width - 200, size.Height - 30), "Fonts/Raleway/Raleway-Regular.ttf", eResourceSource::ENGINE, 32);
	}

	ScreenSpaceTextComponent* textCom = world->GetComponent<ScreenSpaceTextComponent>(com->TextID);
	textCom->SetScreenPosition(Vector2i(size.Width - 200, size.Height - 30));

	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();
	RigidBody2DComponent* rb = world->GetComponent<RigidBody2DComponent>(gameMgrCmp->Player);


	String text;
	text = String("Vel: ") + String::From(rb->GetLinearVelocity().X, 2) + String(" ") + String::From(rb->GetLinearVelocity().Y, 2);

	textCom->SetText(text);
}
