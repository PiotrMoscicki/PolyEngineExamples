#pragma once

#include <Resources/ResourceManager.hpp>
#include <Resources/ResourceBase.hpp>
#include <Collections/Dynarray.hpp>

#pragma warning(disable: 4251)

namespace SGJ
{
	enum class GAME_DLLEXPORT eTileType
	{
		NOTHING = -1,

		STATICGROUND = 0,
		RIGIDBODYGROUND = 1,

		SPIKESBOTTOM = 32,
		SPIKESTOP = 33,
		SPIKESLEFT = 34,
		SPIKESRIGHT = 35,

		PLAYERSTARTPOS = 2,
		PLAYERENDPOS = 3,

		POGOJUMP = 64,
		HIGHJUMP = 65,
		LOWJUMP = 66,

		REVERSEDCONTROLS = 67,
		
		SMALLERCHARACTER = 96,
		LARGERCHARACTER = 97,
		FASTERCHARACTER = 98,

		INVERSEDGRAVITY = 99,

		_COUNT
	};

	class GAME_DLLEXPORT Level : public Poly::BaseObject<>
	{
		friend class LevelComponent;
	public:
		Level(const Poly::String& path);

		Poly::Dynarray<eTileType> Tiles;
		size_t Height;
		size_t Width;
	};
}
