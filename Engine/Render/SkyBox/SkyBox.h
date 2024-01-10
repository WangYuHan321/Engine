#pragma once

#include "AbstractSkyBox.h"
#include "../Cloud/VolumetricClouds.h"
#include "../Program/SkyProgram.h"

namespace Engine
{
	class SkyBox :
		public AbstractSkyBox
	{
	private:
		// Sky cubemap program
		SkyProgram* skyProgram;
		// Cube mesh
		Object* cubeMesh;

		// Cloud system
		CloudSystem::VolumetricClouds* clouds;

		// Render mode
		GLenum renderMode;
	public:
		SkyBox();
		~SkyBox();

		void Render(Camera* camera);
		void NotifyRenderModeUpdate(RenderMode mode);
	private:
		void Initialize();
	};
}



