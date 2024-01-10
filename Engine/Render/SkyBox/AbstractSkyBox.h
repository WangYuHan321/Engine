#pragma once

#include "../Camera/Camera.h"

namespace Engine
{
	class AbstractSkyBox
	{
	public:
		virtual void Render(Camera* cam) = 0;
	};
}


