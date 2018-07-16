#pragma once

namespace Poly { class World; }

namespace SGJ
{
	namespace CameraMovementSystem
	{
		void CameraMovementUpdatePhase(Poly::Scene* world);

		float ElasticEaseOut(float p);
	}
}