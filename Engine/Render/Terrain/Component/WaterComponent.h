#pragma once

#include "TerrainComponent.h"
#include "../../Program/ProceduralWaterProgram.h"

namespace Engine
{
	class WaterComponent :
		public TerrainComponent
	{
	private:

		ProceduralWaterProgram* fillProgram;
		ProceduralWaterProgram* wireProgram;
		ProceduralWaterProgram* pointProgram;
		ProceduralWaterProgram* shadowProgram;

		ProceduralWaterProgram* activeWaterProgram;

		Object* waterTile;

	public:
		WaterComponent();

		virtual void Initialize();

		virtual uint32 GetRenderRadius();

		virtual Program* GetActiveProgram();

		virtual Program* GetShadowMapProgram();

		virtual void PreRenderComponent();
		virtual void RenderComponent(int i, int j, Engine::Camera* camera);
		virtual void RenderShadow(const glm::mat4& projection, int i, int j, Engine::Camera* cam);
		virtual void PostRenderComponent();
		virtual void NotifyRenderModeChange(Engine::RenderMode mode);

	};
}


