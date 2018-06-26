#pragma once

#include <ECS/ComponentBase.hpp>
#include <Collections/Dynarray.hpp>
#include <UniqueID.hpp>
#include <Core.hpp>
#include "Level.hpp"

namespace SGJ
{
	class GAME_DLLEXPORT GameManagerWorldComponent : public Poly::ComponentBase
	{
	public:
		Poly::Entity* Player;
		Poly::Entity* Camera;

		Poly::Dynarray<Poly::Entity*> LevelEntities;
		Poly::Dynarray<Poly::Entity*> OtherEntities;

		Poly::Dynarray<Poly::Entity*> SoundSampleEntities;

		Poly::Dynarray<Level*> Levels;
		size_t CurrentLevelID = 0;
		float MinLevelWidth, MaxLevelWidth;
		float MinLevelHeight, MaxLevelHeight;
	};
}