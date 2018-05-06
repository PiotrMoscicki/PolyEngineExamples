#pragma once

#include <ECS/ComponentBase.hpp>

#include "Level.hpp"

namespace SGJ
{
	class GAME_DLLEXPORT TileComponent : public Poly::ComponentBase
	{
		RTTI_DECLARE_TYPE_DERIVED(TileComponent, Poly::ComponentBase)
		{
			NO_RTTI_PROPERTY()
		}

	public:
		TileComponent(eTileType type) : Type(type) {}

		eTileType GetTileType() const { return Type; }

	private:
		eTileType Type;
	};
}
