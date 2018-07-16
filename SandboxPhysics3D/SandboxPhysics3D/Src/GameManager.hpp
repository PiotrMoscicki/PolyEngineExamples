#pragma once

#include <ECS/World.hpp>

namespace BT
{
	namespace GameManagerSystem
	{
		void InitializeDemoWorld(Poly::Scene* world);

		void SpawnBullet(Poly::Scene* world);

		void Update(Poly::Scene* world);
	}
}