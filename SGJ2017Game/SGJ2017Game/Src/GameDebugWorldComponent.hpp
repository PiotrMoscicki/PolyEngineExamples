#pragma once

#include <ECS/ComponentBase.hpp>

namespace SGJ
{
	class GAME_DLLEXPORT GameDebugWorldComponent : public Poly::ComponentBase
	{
	public:
		Poly::Entity* TextID;
	};
}