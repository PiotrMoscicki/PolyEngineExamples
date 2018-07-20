#pragma once

#include <ECS/World.hpp>
#include <Collections/String.hpp>
#include <Math/Vector.hpp>
#include <Rendering/Particles/ParticleComponent.hpp>

using namespace Poly;

namespace GameManagerSystem
{
	void CreateScene(Scene* world);


	Entity* CreateModel(Scene* world, String path);
	
	void CreateTextUI(Scene* world);
	void CreateCamera(Scene* world);
	void CreatePBRShpereGrid(Scene* world, Vector pos, Color albedo);
	void CreateTranslucent(Scene* world);
	void CreateSponza(Scene* world);
	void CreateSponzaSample(Scene* world);
	
	void CreatePointLights(Scene* world, int quota);
	Entity* CreatePointLight(Scene* world, Vector& position, float Range);
	void CreateSpotLight(Scene* world, float Range);
	void CreateParticles(Scene* world);
	void SpawnSpritesSheets(Scene* world);
	void SpawnShaderball(Scene* world);

	void SpawnSpritesheet11(Scene* world, Vector pos);
	void SpawnSpritesheet22(Scene* world, Vector pos);
	void SpawnSpritesheet44(Scene* world, Vector pos);
	void SpawnSpritesheet42(Scene* world, Vector pos);
	void SpawnSpritesheet41(Scene* world, Vector pos);
	void SpawnSpritesheet44Random(Scene* world, Vector pos);
	void SpawnSpritesheetGandalf(Scene* world, Vector pos);

	ParticleComponent* SpawnEmitterDefault(Scene* world, Vector pos);
	ParticleComponent* SpawnEmitterWorldSpace(Scene* world, Vector pos);
	ParticleComponent* SpawnEmitterLocalSpace(Scene* world, Vector pos);
	ParticleComponent* SpawnEmitterAmbient(Scene* world, Vector pos);
	ParticleComponent* SpawnEmitterAmbientWind(Scene* world, Vector pos);
	ParticleComponent* SpawnEmitterHeart(Scene* world, Vector pos);
	ParticleComponent* SpawnEmitterHeartImpact(Scene* world, Vector pos);
	ParticleComponent* SpawnEmitterHeartImpact2(Scene* world, Vector pos);
	void SpawnHeartSystem(Scene* world);

	void Update(Scene* world);
	void UpdateParticles(Scene* world);
	void UpdateLights(Scene* world);
	void UpdateModel(Scene* world);
	void UpdatePostProcess(Scene* world);
	
	void Deinit(Scene* world);
}
