#pragma once

#include <ECS/ComponentBase.hpp>

#pragma warning(disable: 4251)

namespace SGJ
{
	class GAME_DLLEXPORT GameDebugWorldComponent : public Poly::ComponentBase
	{
	public:
		Poly::Entity* TextID;
	};
}