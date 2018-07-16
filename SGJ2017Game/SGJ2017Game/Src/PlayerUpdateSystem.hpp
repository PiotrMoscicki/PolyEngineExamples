#pragma once


namespace Poly
{
	class World;
	class RigidBody2DComponent;
	class Vector;
}

namespace SGJ
{
	class PlayerControllerComponent;
	enum class ePowerup;

	namespace PlayerUpdateSystem
	{
		void Update(Poly::Scene* world);

		void PlayerAlive(Poly::World * world, SGJ::PlayerControllerComponent * playerCmp, double deltaTime, Poly::RigidBody2DComponent * rigidbodyCmp, double time);

		void PlayerDead(Poly::World * world, double time, SGJ::PlayerControllerComponent * playerCmp);

		float ElasticEaseOut(float p);

		void KillPlayer(Poly::Scene* world);
		void ResetPlayer(Poly::Scene* world, const Poly::Vector& spawnLocation);
		void TryPlayerJump(Poly::Scene* world);
		void UpdateInAir(Poly::Scene* world);
		void ProcessJumpStrech(Poly::Scene* world);
		void PickupPowerup(Poly::Scene* world, ePowerup powerup);
		void UpdateDeathAction(Poly::Scene* world);
		void PushPlayer(Poly::Scene* world, const Poly::Vector& normal, float force);
	}

}