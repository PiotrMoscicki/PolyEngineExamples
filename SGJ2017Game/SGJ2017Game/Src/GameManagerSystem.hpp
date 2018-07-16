#pragma once

#include <ECS/World.hpp>
#include <Collections/String.hpp>
#include <Math/Vector.hpp>

namespace SGJ {
	enum class eTileType;
}

namespace Poly
{
	enum class eRigidBody2DType;
	class String;
}

namespace SGJ
{
	namespace GameManagerSystem
	{
		void Update(Poly::Scene*);

		Poly::Entity* CreateTileObject(Poly::Scene* world, const Poly::Vector& position, eTileType tileType, Poly::String meshSource, Poly::eRigidBody2DType physicsPropertie, const Poly::Vector& size, const Poly::Color& color, bool colliding);
		Poly::Entity* SpawnPlayer(Poly::Scene* world, const Poly::Vector& position);

		void LoadLevel(Poly::Scene* world, const Poly::String& path);
		void SpawnLevel(Poly::Scene* world, size_t idx);
		void DespawnLevel(Poly::Scene* world);

		void PlaySample(Poly::Scene * world, const Poly::String& file, const Poly::Vector& position, float pitch = 1.0f, float gain = 1.0f);

		void PrepareNonlevelObjects(Poly::Scene* world);
		void Cleanup(Poly::Scene* world);
	}
}