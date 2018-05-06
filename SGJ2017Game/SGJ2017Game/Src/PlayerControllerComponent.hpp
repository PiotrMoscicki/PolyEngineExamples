#pragma once

#include "ECS/ComponentBase.hpp"
#include "PlayerUpdateSystem.hpp"
#include "Powerups.hpp"

namespace SGJ
{
	class GAME_DLLEXPORT PlayerControllerComponent : public Poly::ComponentBase
	{
		RTTI_DECLARE_TYPE_DERIVED(PlayerControllerComponent, Poly::ComponentBase)
		{
			NO_RTTI_PROPERTY()
		}

	public:

		PlayerControllerComponent(float movementSpeed = 12.0f, float jumpForce = 5.8f);

		float GetMovementSpeed() const { return MovementSpeed; }
		float GetJumpForce() const { return JumpForce; }
		bool IsInAir() const { return InAir; }
		bool GetHasChangedSize() const { return HasChangedSize; }
		ePowerup GetActivePowerup() const { return ActivePowerup; }
		Poly::Vector GetMoveVector() const { return CurrentMoveVector; }
		Poly::Vector GetDefaultScale() const { return DefaultScale; }
		float GetDensityMultiplier() const { return DensityMultiplier; }
		float GetDefaultJumpForce() const { return DefJumpForce; }
		
		void SetHasChangedSize(bool hasChangedSize) { HasChangedSize = hasChangedSize; }
		void SetMoveVector(Poly::Vector newMoveVector) { CurrentMoveVector = newMoveVector; }
		void SetDensityMultiplier(float densityMul) { DensityMultiplier = densityMul; }
		void SetActivePowerup(ePowerup newPower) { ActivePowerup = newPower; };
		void SetJumpForce(float jumpForce) { JumpForce = jumpForce; }

		Poly::Vector SpawnPoint;

		// Jumping logic
		bool InAir = false;
		float JumpCooldownTimer = 0.f;
		float DeathCoolDowntime = 0.f;
		double LastLandTimeStart = 0;
		double LastJumpTimeStart = 0;
		float DeathCoolDowntimeMax = 2.0f;
		
	private:
		const float DefJumpForce;
		float MovementSpeed = 50;
		float JumpForce = 250;
		float DensityMultiplier = 1;
		bool HasChangedSize = false;
		ePowerup ActivePowerup = ePowerup::NONE;
		Poly::Vector CurrentMoveVector = Poly::Vector::ZERO;
		Poly::Vector DefaultScale = Poly::Vector(0.8f, 0.8f, 0.8f);
	};
}
