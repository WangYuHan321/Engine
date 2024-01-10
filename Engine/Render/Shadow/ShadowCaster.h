#pragma once

#include "../Camera/Camera.h"

namespace Engine
{
	class ShadowCaster
	{
	public:
		virtual void RenderShadow(Camera* camera, const glm::mat4& projectionMatrix) = 0;
	};

}


