#include "CameraMovementSystem.hpp"

#include <ECS/World.hpp>
#include <Time/TimeSystem.hpp>
#include <Rendering/Camera/CameraComponent.hpp>
#include "Physics2D/Rigidbody2DComponent.hpp"
#include "CameraMovementComponent.hpp"
#include <Rendering/PostprocessSettingsComponent.hpp>

#include "GameManagerWorldComponent.hpp"

using namespace Poly;
using namespace SGJ;

void SGJ::CameraMovementSystem::CameraMovementUpdatePhase(Poly::Scene* world)
{
	auto deltaTime = float(TimeSystem::GetTimerDeltaTime(world, Poly::eEngineTimer::GAMEPLAY));
	//double time = TimeSystem::GetTimerElapsedTime(world, Poly::eEngineTimer::GAMEPLAY);

	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();
	Entity* player = gameMgrCmp->Player;
	EntityTransform& playerTrans = player->GetTransform();
	RigidBody2DComponent* rigidbodyCmp = world->GetComponent<RigidBody2DComponent>(player);

	for (auto tuple : world->IterateComponents<CameraComponent, CameraMovementComponent>())
	{
		CameraComponent* cameraCmp = std::get<CameraComponent*>(tuple);
		CameraMovementComponent* cameraMvmtCmp = std::get<CameraMovementComponent*>(tuple);
		EntityTransform& transform = cameraCmp->GetTransform();

		float velocity = rigidbodyCmp->GetLinearVelocity().LengthSquared();
		Angle TargetFov = Lerp(60_deg, 47_deg, Clamp(velocity/350.0f, 0.0f, 1.0f));
		cameraCmp->SetTargetFOV(TargetFov);
		cameraCmp->SetFOV(Lerp(cameraCmp->GetFOV(), cameraCmp->GetTargetFOV(), deltaTime));

		const float cameraHeight = 16.f;

		Vector TargetPosition;
		TargetPosition.Z = 8.0f / Tan(cameraCmp->GetFOV() / 2.0f);

		float verticalSpan = cameraHeight / 2.0f;
		float horizontalSpan = (cameraHeight * cameraCmp->GetAspect()) / 2.0f;
		float minW = gameMgrCmp->MinLevelWidth + horizontalSpan;
		float maxW = gameMgrCmp->MaxLevelWidth - horizontalSpan + 1;
		float minH = gameMgrCmp->MinLevelHeight + verticalSpan;
		float maxH = gameMgrCmp->MaxLevelHeight - verticalSpan + 1;

		if (minH > maxH)
			TargetPosition.Y = minH;
		else
			TargetPosition.Y = Clamp(playerTrans.GetGlobalTranslation().Y, minH, maxH) + 0.5f;

		if (minW > maxW)
			TargetPosition.X = minW;
		else
			TargetPosition.X = Clamp(playerTrans.GetGlobalTranslation().X, minW, maxW) - 0.5f;

		// add lag to translation
		cameraMvmtCmp->SetTargetTranslation(TargetPosition);
		Vector Translation = Lerp(transform.GetLocalTranslation(), cameraMvmtCmp->GetTargetTranslation(), 2.0f*deltaTime);

		// sum the camera modified translation
		transform.SetLocalTranslation(Translation);

		//PostprocessSettingsComponent* post = cameraCmp->GetSibling<PostprocessSettingsComponent>();
		//if (post != nullptr)
		//{
		//	float Time = (float)TimeSystem::GetTimerElapsedTime(world, eEngineTimer::GAMEPLAY);
		//	float intpart;
		//	float fractpart = std::modf(Time, &intpart);
		//	// post->UseCashetes = (fractpart > 0.5) ? 1 : 0;
		//	// gConsole.LogInfo("SGJ::CameraMovementSystem::CameraMovementUpdatePhase Time: {}, useCashetes: {}", Time, post->UseCashetes);
		//}
	}

}

float SGJ::CameraMovementSystem::ElasticEaseOut(float p)
{
	return std::sin(-13.f * 3.14f * (p + 1.f)) * std::pow(2.f, -10.f * p) + 1.f;
}
