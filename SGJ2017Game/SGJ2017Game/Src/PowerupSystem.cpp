#include "Physics2D/Physics2DColliders.hpp"
#include "PowerupSystem.hpp"
#include "PlayerControllerComponent.hpp"
#include "PlayerUpdateSystem.hpp"
#include "ECS/World.hpp"
#include "Powerups.hpp"
#include "Physics2D/Rigidbody2DComponent.hpp"
#include "Time/TimeSystem.hpp"
#include "Physics2D/Physics2DWorldComponent.hpp"

using namespace Poly;

namespace SGJ
{
	//this function checks which powerup is active and applies its effects
	//TODO it should probably be split into few smaller functions
	void PowerupSystem::ApplyPowerupsAndInput(Scene* world, PlayerControllerComponent* playerCmp)
	{
		Poly::RigidBody2DComponent* rbCmp = playerCmp->GetSibling<Poly::RigidBody2DComponent>();
		//double deltaTime = TimeSystem::GetTimerDeltaTime(world, Poly::eEngineTimer::GAMEPLAY);
		bool changedSize = false;
		//bool changedJumpHeight = false;

		ePowerup activePower = playerCmp->GetActivePowerup();
		Poly::Vector move = playerCmp->GetMoveVector();

		float speedConstraint = 10.0f;

		//reset jump force to default (in case no jump modifier)
		playerCmp->SetJumpForce(playerCmp->GetDefaultJumpForce());

		//check active powerup and modify relevant mechanics
		switch (activePower)
		{
		case(ePowerup::REVERSED_CONTROLS):
			move.X *= -1;
			break;
		case(ePowerup::HIGH_JUMP):
			playerCmp->SetJumpForce(playerCmp->GetDefaultJumpForce() * 2.0f);
			break;
		case(ePowerup::LOW_JUMP):
			playerCmp->SetJumpForce(playerCmp->GetDefaultJumpForce() * 0.7f);
			break;
		case(ePowerup::POGO_JUMP):
			PlayerUpdateSystem::TryPlayerJump(world);
			break;
		case(ePowerup::INCREASED_SPEED):
			move.X *= 2;
			speedConstraint *= 2;
			break;
		case(ePowerup::INCREASED_SIZE):
		case(ePowerup::DECREASED_SIZE):
			changedSize = true;
			break;
		default:
			break;
		}

		if (activePower == ePowerup::INVERSED_GRAVITY)
		{
			world->GetWorldComponent<Physics2DWorldComponent>()->SetGravity(Vector(0, 9.81f, 0));
			move.Y *= -1;
		}
		else
			world->GetWorldComponent<Physics2DWorldComponent>()->SetGravity(Vector(0, -9.81f, 0));


		//check if size changed in this frame
		if ((!changedSize && playerCmp->GetHasChangedSize() ) || (changedSize && !playerCmp->GetHasChangedSize()))
		{
			Circle2DColliderComponent* colliderCmp = playerCmp->GetSibling<Circle2DColliderComponent>();

			Poly::Vector size;
			float colliderRange;
			if (activePower == ePowerup::INCREASED_SIZE)
			{
				playerCmp->SetHasChangedSize(true);
				playerCmp->SetDensityMultiplier(4.f);		//Setting size density multiplier
				size = playerCmp->GetDefaultScale() * 2.0f;
				colliderRange = playerCmp->GetDefaultScale().X * 1.5f;
			}
			else if(activePower == ePowerup::DECREASED_SIZE)
			{
				playerCmp->SetHasChangedSize(true);
				playerCmp->SetDensityMultiplier(.7f);		//Setting decr size density multiplier
				size = playerCmp->GetDefaultScale() * 0.5f;
				colliderRange = playerCmp->GetDefaultScale().X * 0.25f;
			}
			else
			{
				//powerup changed from size-changing to another
				//reset player size to default
				playerCmp->SetHasChangedSize(false);
				playerCmp->SetDensityMultiplier(1.0f);
				size = playerCmp->GetDefaultScale();
				colliderRange = playerCmp->GetDefaultScale().X * 0.5f;;
			}
			playerCmp->GetTransform().SetLocalScale(size);
			colliderCmp->SetSize(colliderRange);
			rbCmp->SetDensity(playerCmp->GetDensityMultiplier());


		}

		//apply validated movement vector
		if (rbCmp->GetLinearVelocity().Length() < speedConstraint && move.Length() > 0)
			rbCmp->ApplyImpulseToCenter(move);
		playerCmp->SetMoveVector(Vector::ZERO);
	}
}
