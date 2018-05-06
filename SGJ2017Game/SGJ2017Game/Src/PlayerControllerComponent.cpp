#include "PlayerControllerComponent.hpp"

using namespace Poly;

RTTI_DEFINE_TYPE(SGJ::PlayerControllerComponent)

namespace SGJ
{
	PlayerControllerComponent::PlayerControllerComponent(float movementSpeed, float jumpForce) :
		DefJumpForce(jumpForce),
		MovementSpeed(movementSpeed),
		JumpForce(jumpForce)
	{
	}
}
