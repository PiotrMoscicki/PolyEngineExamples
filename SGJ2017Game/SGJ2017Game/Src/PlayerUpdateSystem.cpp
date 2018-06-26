#include "PlayerUpdateSystem.hpp"
#include "ECS/World.hpp"
#include "Input/InputWorldComponent.hpp"
#include "PlayerControllerComponent.hpp"
#include "Physics2D/Rigidbody2DComponent.hpp"
#include "PowerupSystem.hpp"
#include "Time/Timer.hpp"
#include "Physics2D/Physics2DWorldComponent.hpp"
#include "GameManagerWorldComponent.hpp"
#include "Rendering/PostprocessSettingsComponent.hpp"
#include "GameManagerSystem.hpp"

using namespace Poly;

namespace SGJ
{
	void PlayerUpdateSystem::Update(World* world)
	{
		double deltaTime = TimeSystem::GetTimerDeltaTime(world, Poly::eEngineTimer::GAMEPLAY);

		GameManagerWorldComponent* mgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();
		PlayerControllerComponent* playerCmp = world->GetComponent<PlayerControllerComponent>(mgrCmp->Player);

		if (playerCmp->DeathCoolDowntime > 0)
			UpdateDeathAction(world);
		else
		{

			// Jumping
			UpdateInAir(world);
			if (world->GetWorldComponent<InputWorldComponent>()->IsPressed(eKey::SPACE))
				TryPlayerJump(world);
			ProcessJumpStrech(world);

			// Moving
			const float modifier = (playerCmp->InAir ? 0.2f : 1.0f);
			if (world->GetWorldComponent<InputWorldComponent>()->IsPressed(eKey::KEY_A) || world->GetWorldComponent<InputWorldComponent>()->IsPressed(eKey::LEFT))
				playerCmp->SetMoveVector(Vector(float(-deltaTime * modifier * playerCmp->GetMovementSpeed()), 0, 0));
			if (world->GetWorldComponent<InputWorldComponent>()->IsPressed(eKey::KEY_D) || world->GetWorldComponent<InputWorldComponent>()->IsPressed(eKey::RIGHT))
				playerCmp->SetMoveVector(Vector(float(deltaTime * modifier * playerCmp->GetMovementSpeed()), 0, 0));
			PowerupSystem::ApplyPowerupsAndInput(world, playerCmp);


			if (world->GetWorldComponent<InputWorldComponent>()->IsPressed(eKey::ESCAPE))
				ResetPlayer(world, world->GetComponent<PlayerControllerComponent>(mgrCmp->Player)->SpawnPoint);

			if (world->GetWorldComponent<InputWorldComponent>()->IsPressed(eKey::KEY_N))
			{
				mgrCmp->CurrentLevelID = (mgrCmp->CurrentLevelID + 1) % mgrCmp->Levels.GetSize();
				GameManagerSystem::SpawnLevel(world, mgrCmp->CurrentLevelID);
			}
		}
	}

	void PlayerUpdateSystem::KillPlayer(Poly::World * world)
	{
		GameManagerWorldComponent* manager = world->GetWorldComponent<GameManagerWorldComponent>();
		PlayerControllerComponent* playerCmp = world->GetComponent<PlayerControllerComponent>(manager->Player);
		GameManagerSystem::PlaySample(world, "Audio/death-sound.ogg", manager->Player->GetTransform().GetGlobalTranslation(), 1.0f, 1.8f);
		playerCmp->DeathCoolDowntime = playerCmp->DeathCoolDowntimeMax;
	}

	void PlayerUpdateSystem::ResetPlayer(Poly::World* world, const Vector& spawnLocation)
	{
		GameManagerWorldComponent* manager = world->GetWorldComponent<GameManagerWorldComponent>();
		PlayerControllerComponent* playerCmp = world->GetComponent<PlayerControllerComponent>(manager->Player);
		RigidBody2DComponent* rbCmp = world->GetComponent<RigidBody2DComponent>(manager->Player);

		manager->Player->GetTransform().SetLocalTranslation(spawnLocation);
		playerCmp->SpawnPoint = spawnLocation;

		rbCmp->SetLinearVelocity(Vector::ZERO);
		rbCmp->SetAngularVelocity(0.f);

		playerCmp->InAir = false;
		playerCmp->JumpCooldownTimer = 0.f;
		playerCmp->DeathCoolDowntime = 0.f;

		playerCmp->SetActivePowerup(ePowerup::NONE);

		rbCmp->UpdatePosition();
	}

	void PlayerUpdateSystem::TryPlayerJump(Poly::World* world)
	{
		Entity* player = world->GetWorldComponent<GameManagerWorldComponent>()->Player;

		RigidBody2DComponent* rigidbodyCmp = world->GetComponent<RigidBody2DComponent>(player);
		PlayerControllerComponent* playerCmp = world->GetComponent<PlayerControllerComponent>(player);

		if (playerCmp->JumpCooldownTimer >= 0 || playerCmp->InAir)
			return;

		// we can jump indeed
		double time = TimeSystem::GetTimerElapsedTime(world, Poly::eEngineTimer::GAMEPLAY);
		playerCmp->LastJumpTimeStart = time;
		playerCmp->JumpCooldownTimer = 0.3f;
		playerCmp->InAir = true;

		Vector jump(0, 0, 0);
		jump.Y = playerCmp->GetJumpForce();

		if (playerCmp->GetActivePowerup() == ePowerup::INVERSED_GRAVITY)
			jump.Y = -jump.Y;

		rigidbodyCmp->ApplyImpulseToCenter(jump);
		GameManagerSystem::PlaySample(world, "Audio/jump-sound.ogg", player->GetTransform().GetGlobalTranslation(), 1.5, 1.5);
	}

	void PlayerUpdateSystem::UpdateInAir(Poly::World* world)
	{
		GameManagerWorldComponent* mgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();
		PlayerControllerComponent* playerCmp = world->GetComponent<PlayerControllerComponent>(mgrCmp->Player);
		RigidBody2DComponent* rigidbodyCmp = world->GetComponent<RigidBody2DComponent>(mgrCmp->Player);
		auto deltaTime = float(TimeSystem::GetTimerDeltaTime(world, Poly::eEngineTimer::GAMEPLAY));

		if(playerCmp->JumpCooldownTimer >= 0)
			playerCmp->JumpCooldownTimer -= deltaTime;

		if (playerCmp->JumpCooldownTimer > 0)
			return;

		bool wasInAir = playerCmp->InAir;
		playerCmp->InAir = true;
		Physics2DWorldComponent* physicsWorldComponent = world->GetWorldComponent<Physics2DWorldComponent>();

		for (Physics2DWorldComponent::Collision col : physicsWorldComponent->GetCollidingBodies(rigidbodyCmp))
			if (playerCmp->GetActivePowerup() == ePowerup::INVERSED_GRAVITY ? col.Normal.Dot(Vector::UNIT_Y) > 0.5 : col.Normal.Dot(Vector::UNIT_Y) < -0.5)
			{
				if (wasInAir)
					playerCmp->LastLandTimeStart = TimeSystem::GetTimerElapsedTime(world, Poly::eEngineTimer::GAMEPLAY);

				playerCmp->InAir = false;
				break;
			}

		if(!playerCmp->InAir)
			rigidbodyCmp->SetLinearDamping(3.f);
		else
			rigidbodyCmp->SetLinearDamping(0.1f);
	}

	float PlayerUpdateSystem::ElasticEaseOut(float p)
	{
		return std::sin(-13.f * 3.14f * (p + 1.f)) * std::pow(2.f, -10.f * p) + 1.f;
	}

	void PlayerUpdateSystem::ProcessJumpStrech(Poly::World * world)
	{
		GameManagerWorldComponent* mgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();
		PlayerControllerComponent* playerCmp = world->GetComponent<PlayerControllerComponent>(mgrCmp->Player);
		double time = TimeSystem::GetTimerElapsedTime(world, Poly::eEngineTimer::GAMEPLAY);
		EntityTransform& playerTrans = playerCmp->GetTransform();
		auto timeSinceLastJump = float(time - playerCmp->LastJumpTimeStart);
		auto timeSinceLastLand = float(time - playerCmp->LastLandTimeStart);

		if (timeSinceLastLand < timeSinceLastJump)
		{
			// stretch on jump anim
			float tX = 1.0f * timeSinceLastJump;
			//float tY = 1.0f * timeSinceLastJump;
			float scaleX = Lerp(2.5f, 1.0f, Clamp(ElasticEaseOut(tX), 0.0f, 1.0f));
			// float scaleY = Lerp(0.3f, 1.0f, Clamp(ElasticEaseOut(tY), 0.0f, 1.0f));
			// playerTrans->SetLocalScale(playerTrans->GetGlobalRotation().GetConjugated() * Vector(scaleX, scaleY, 1.0f));
			playerTrans.SetLocalScale(Vector(scaleX, scaleX, scaleX));
		}
		else
		{
			// stretch on jump anim
			float tX = 0.75f * timeSinceLastJump;
			//float tY = 0.5f * timeSinceLastJump;
			float scaleX = Lerp(0.3f, 1.5f, Clamp(ElasticEaseOut(tX), 0.0f, 1.0f));
			// float scaleY = Lerp(2.5f, 1.2f, Clamp(ElasticEaseOut(tY), 0.0f, 1.0f));
			// playerTrans->SetLocalScale(playerTrans->GetGlobalRotation().GetConjugated() * Vector(scaleX, scaleY, 1.0f));
			playerTrans.SetLocalScale(Vector(scaleX, scaleX, scaleX));
		}
	}

	void PlayerUpdateSystem::PickupPowerup(Poly::World * world, ePowerup powerup)
	{
		GameManagerWorldComponent* mgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();
		PlayerControllerComponent* playerCmp = world->GetComponent<PlayerControllerComponent>(mgrCmp->Player);
		EntityTransform& playerTrans = playerCmp->GetTransform();
		if (playerCmp->GetActivePowerup() != powerup)
		{
			playerCmp->SetActivePowerup(powerup);
			GameManagerSystem::PlaySample(world, "Audio/powerup.ogg", playerTrans.GetGlobalTranslation(), 1.0f, 1.5f);
		}
	}

	void PlayerUpdateSystem::UpdateDeathAction(Poly::World * world)
	{
		GameManagerWorldComponent* mgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();
		PlayerControllerComponent* playerCmp = world->GetComponent<PlayerControllerComponent>(mgrCmp->Player);
		//RigidBody2DComponent* rigidbodyCmp = world->GetComponent<RigidBody2DComponent>(mgrCmp->Player);
		PostprocessSettingsComponent* postCmp = world->GetComponent<PostprocessSettingsComponent>(mgrCmp->Camera);
		auto deltaTime = float(TimeSystem::GetTimerDeltaTime(world, Poly::eEngineTimer::GAMEPLAY));

		if (playerCmp->DeathCoolDowntime >= 0)
		{
			if (postCmp)
			{
				float t = Clamp(playerCmp->DeathCoolDowntime / playerCmp->DeathCoolDowntimeMax, 0.0f, 1.0f);
				postCmp->Saturation = Clamp(Lerp(1.0f, -1.0f, 2.0f*t), 0.0f, 1.0f);
				postCmp->Vignette = Clamp(Lerp(0.15f, 3.0f, 2.0f*t), 0.0f, 3.0f);
				postCmp->Distortion = Clamp(Lerp(0.25f, 0.4f, 4.0f*t), 0.25f, 0.4f);
				postCmp->Grain = Lerp(0.05f, 0.3f, t);
				postCmp->Stripes = Lerp(0.15f, 0.35f, t);
			}

			playerCmp->DeathCoolDowntime -= deltaTime;
		}

		if (playerCmp->DeathCoolDowntime <= 0)
		{
			if (postCmp)
			{
				postCmp->Saturation = 1.0f;
				postCmp->Vignette = 0.15f;
				postCmp->Distortion = 0.05f;
				postCmp->Grain = 0.05f;
				postCmp->Stripes = 0.15f;
			}
			ResetPlayer(world, playerCmp->SpawnPoint);
		}
	}

	void PlayerUpdateSystem::PushPlayer(Poly::World* world, const Poly::Vector& normal, float force)
	{
		GameManagerWorldComponent* manager = world->GetWorldComponent<GameManagerWorldComponent>();
		RigidBody2DComponent* rbCmp = world->GetComponent<RigidBody2DComponent>(manager->Player);
		rbCmp->ApplyImpulseToCenter(normal * force);
	}
}
