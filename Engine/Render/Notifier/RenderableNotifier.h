#pragma once

#include "../../Config/WorldConfig.h"
#include "glm/glm.hpp"

namespace Engine 
{
	class RenderableNotifier : public CSingleton<RenderableNotifier>
	{
	private:
		std::list<IRenderable*> renderables;
		std::list<Light*> lights;

		unsigned int previousDrawingMethod;
		glm::vec3 previousLightColor;
		glm::vec3 previousLightDir;

	public:

		RenderableNotifier();
		~RenderableNotifier() {}

		void RegisterRenderable(IRenderable* renderable);
		void RegisterLight(Light* light);
		void CheckUpdateConfig();

	};
}

