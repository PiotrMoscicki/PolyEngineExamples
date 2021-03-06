#include "GameManager.hpp"

#include <Configs/ConfigBase.hpp>
#include <ECS/DeferredTaskSystem.hpp>
#include <ECS/Entity.hpp>
#include <Input/InputSystem.hpp>
#include <Input/InputWorldComponent.hpp>
#include <Physics3D/Collider3DComponent.hpp>
#include <Physics3D/Rigidbody3DComponent.hpp>
#include <Physics3D/Physics3DShapes.hpp>
#include <Physics3D/Physics3DSystem.hpp>
#include <Physics3D/Physics3DWorldComponent.hpp>
#include <Rendering/PostprocessSettingsComponent.hpp>
#include <Rendering/MeshRenderingComponent.hpp>
#include <Rendering/ViewportWorldComponent.hpp>
#include <Rendering/Camera/CameraComponent.hpp>
#include <Rendering/Lighting/LightSourceComponent.hpp>
#include <Movement/FreeFloatMovementComponent.hpp>

#include "GameManagerWorldComponent.hpp"
#include "StatusFlagsComponent.hpp"

using namespace Poly;

void BT::GameManagerSystem::InitializeDemoWorld(Scene* world)
{
	GameManagerWorldComponent* gameManager = gEngine->GetActiveScene()->GetWorldComponent<GameManagerWorldComponent>();
	
	// create camera
	gameManager->Camera = DeferredTaskSystem::SpawnEntityImmediate(gEngine->GetActiveScene());
	DeferredTaskSystem::AddComponentImmediate<FreeFloatMovementComponent>(gEngine->GetActiveScene(), gameManager->Camera, 25, 0.01);
	DeferredTaskSystem::AddComponentImmediate<Poly::CameraComponent>(gEngine->GetActiveScene(), gameManager->Camera, 60_deg, 1.0f, 1000.f);
	gEngine->GetActiveScene()->GetWorldComponent<ViewportWorldComponent>()->SetCamera(0, gEngine->GetActiveScene()->GetComponent<CameraComponent>(gameManager->Camera));
	DeferredTaskSystem::AddComponentImmediate<PostprocessSettingsComponent>(gEngine->GetActiveScene(), gameManager->Camera);
	gameManager->Camera->GetTransform().SetLocalTranslation(Vector(0, 15, 40));
	gameManager->Camera->GetTransform().SetLocalRotation(Quaternion({ Angle::FromDegrees(-15), Angle::FromDegrees(0), Angle::FromDegrees(0) }));
	gEngine->GetActiveScene()->GetComponent<PostprocessSettingsComponent>(gameManager->Camera)->UseFgShader = false;
	gEngine->GetActiveScene()->GetComponent<PostprocessSettingsComponent>(gameManager->Camera)->UseBgShader = false;
	
	// create light
	world->GetWorldComponent<AmbientLightWorldComponent>()->SetColor(Color(0.2f, 0.5f, 1.0f));
	world->GetWorldComponent<AmbientLightWorldComponent>()->SetIntensity(0.5f);
	Quaternion DirLightRot = Quaternion(Vector::UNIT_Y, 80_deg) * Quaternion(Vector::UNIT_X, -80_deg);
	
	gameManager->DirectionalLight = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<StatusFlagsComponent>(gEngine->GetActiveScene(), gameManager->Camera);
	DeferredTaskSystem::AddComponentImmediate<Poly::DirectionalLightComponent>(world, gameManager->DirectionalLight, Color(1.0f, 0.9f, 0.8f), 0.8f);
	gameManager->DirectionalLight->GetTransform().SetLocalRotation(DirLightRot);
	
	// create ground
	gameManager->Ground = DeferredTaskSystem::SpawnEntityImmediate(gEngine->GetActiveScene());
	MeshRenderingComponent* groundMesh = DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, gameManager->Ground, "Models/Ground.fbx", eResourceSource::GAME);
	groundMesh->SetMaterial(0, Material(Color(1.f, 0.5f, 0.2f), Color(1.f, 0.5f, 0.2f), 1.0f, 1.0f, 0.5f));
	groundMesh->SetShadingModel(eShadingModel::PBR);
	gameManager->Ground->GetTransform().SetLocalTranslation(Vector(0, 0, 0));
	gameManager->Ground->GetTransform().SetLocalRotation(Quaternion({ -90_deg, 0_deg, 0_deg }));
	
	// Collider3DComponentTemplate* colliderTemplate = new Collider3DComponentTemplate();
	// colliderTemplate->Shape = std::make_shared<Physics3DShape>(*reinterpret_cast<Physics3DShape*>(new Physics3DBoxShape(Vector(20, 20, 0))));
	// DeferredTaskSystem::AddComponentImmediate<Collider3DComponent>(gEngine->GetActiveScene(), gameManager->Ground, gEngine->GetActiveScene(), *colliderTemplate);
	
	Rigidbody3DComponentTemplate* rigidbodyTemplate = new Rigidbody3DComponentTemplate();
	rigidbodyTemplate->RigidbodyType = eRigidBody3DType::STATIC;
	DeferredTaskSystem::AddComponentImmediate<Rigidbody3DComponent>(gEngine->GetActiveScene(), gameManager->Ground, gEngine->GetActiveScene(), *rigidbodyTemplate);
	
	// build the wall
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			Entity* brick = DeferredTaskSystem::SpawnEntityImmediate(gEngine->GetActiveScene());
			gameManager->Bricks.PushBack(brick);
			brick->GetTransform().SetLocalTranslation(Vector(-5 + i * 2.f, 5 + j * 2.f, 0.f));
			// colliderTemplate = new Collider3DComponentTemplate();
			// colliderTemplate->Shape = std::make_shared<Physics3DShape>(*reinterpret_cast<Physics3DShape*>(new Physics3DBoxShape(Vector(1, 1, 1))));
			// DeferredTaskSystem::AddComponentImmediate<Collider3DComponent>(gEngine->GetActiveScene(), brick, gEngine->GetActiveScene(), *colliderTemplate);
			rigidbodyTemplate = new Rigidbody3DComponentTemplate();
			rigidbodyTemplate->Mass = 10;
			DeferredTaskSystem::AddComponentImmediate<Rigidbody3DComponent>(gEngine->GetActiveScene(), brick, gEngine->GetActiveScene(), *rigidbodyTemplate);
			MeshRenderingComponent* brickMesh = DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, brick, "Models/Cube.fbx", eResourceSource::GAME);
			brickMesh->SetMaterial(0, Material(Color(1.0f, 0.0f, 0.2f), Color(1.0f, 0.0f, 0.2f), 1.0f, 1.0f, 0.5f));
			brickMesh->SetShadingModel(eShadingModel::PBR);
		}
	}
}

void BT::GameManagerSystem::SpawnBullet(Poly::Scene* world)
{
	GameManagerWorldComponent* gameManager = Poly::gEngine->GetActiveScene()->GetWorldComponent<GameManagerWorldComponent>();
	
	Entity* bullet = DeferredTaskSystem::SpawnEntityImmediate(gEngine->GetActiveScene());
	gameManager->Bullets.PushBack(bullet);
	
	DeferredTaskSystem::AddComponentImmediate<StatusFlagsComponent>(gEngine->GetActiveScene(), bullet);
	// DeferredTaskSystem::AddComponentImmediate<TransformComponent>(gEngine->GetActiveScene(), bullet);
	// Collider3DComponentTemplate* colliderTemplate = new Collider3DComponentTemplate();
	// colliderTemplate->Shape = std::make_shared<Physics3DShape>(*reinterpret_cast<Physics3DShape*>(new Physics3DSphereShape(1)));
	// DeferredTaskSystem::AddComponentImmediate<Collider3DComponent>(gEngine->GetActiveScene(), bullet, gEngine->GetActiveScene(), *colliderTemplate);
	Rigidbody3DComponentTemplate* rigidbodyTemplate = new Rigidbody3DComponentTemplate();
	rigidbodyTemplate->Mass = 10;
	rigidbodyTemplate->Registered = true;
	DeferredTaskSystem::AddComponentImmediate<Rigidbody3DComponent>(gEngine->GetActiveScene(), bullet, gEngine->GetActiveScene(), *rigidbodyTemplate);
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, bullet, "Models/Sphere.fbx", eResourceSource::GAME);
	
	world->GetComponent<MeshRenderingComponent>(bullet)->SetMaterial(0, Material(Color(1.0f, 0.2f, 0.0f), Color(1.0f, 0.2f, 0.0f), 1.0f, 1.0f, 0.5f));
	world->GetComponent<MeshRenderingComponent>(bullet)->SetShadingModel(eShadingModel::PBR);
}

void BT::GameManagerSystem::Update(Scene* world)
{
	GameManagerWorldComponent* gameManager = Poly::gEngine->GetActiveScene()->GetWorldComponent<GameManagerWorldComponent>();
	// Physics3DWorldComponent* physicsWorldComponent = Poly::gEngine->GetActiveScene()->GetWorldComponent<Physics3DWorldComponent>();
	InputWorldComponent* input = world->GetWorldComponent<InputWorldComponent>();

	// for(auto tuple : world->IterateComponents<TransformComponent>())
	for (auto entity : gameManager->Bricks)
	{
		if (entity->GetTransform().GetGlobalTranslation().Length() > 100.0f && entity->GetUUID() != gameManager->Camera->GetUUID())
		{
			DeferredTaskSystem::DestroyEntity(world, entity);
			if (gameManager->Bricks.Contains(entity))
				gameManager->Bricks.Remove(entity);
		}
	}

	for (auto entity : gameManager->Bullets)
	{
		if (entity->GetTransform().GetGlobalTranslation().Length() > 100.0f && entity->GetUUID() != gameManager->Camera->GetUUID())
		{
			DeferredTaskSystem::DestroyEntity(world, entity);

			if (gameManager->Bullets.Contains(entity))
				gameManager->Bullets.Remove(entity);
		}
	}

	for (size_t i = 0; i < gameManager->Bullets.GetSize(); i++)
	{
		StatusFlagsComponent* bulletStatus = world->GetComponent<StatusFlagsComponent>(gameManager->Bullets[i]);
		// Rigidbody3DComponent* rigidbody = bulletStatus->GetSibling<Rigidbody3DComponent>();
		if (!bulletStatus->PhysicsInitialized)
		{
			EntityTransform& transform = gameManager->Camera->GetTransform();
			// Vector direction = MovementSystem::GetGlobalForward(gameManager->Camera->GetTransform());
	
			gameManager->Bullets[i]->GetTransform().SetLocalTranslation(transform.GetGlobalTranslation());
	
			// rigidbody->UpdatePosition();
			// rigidbody->ApplyGravity();
			// rigidbody->ApplyImpulseToCenter(direction * 1000);
	
			//Physics3DSystem::RegisterRigidbody(world, gameManager->Bullets[i], eCollisionGroup::RIGIDBODY, eCollisionGroup::RIGIDBODY);
	
			bulletStatus->PhysicsInitialized = true;
		}
	}
	
	if (input->IsClicked(eMouseButton::RIGHT))
		SpawnBullet(world);
}