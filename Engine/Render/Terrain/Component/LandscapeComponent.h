#pragma once
#include "TerrainComponent.h"
#include "../../Program/ProceduralTerrainProgram.h"

namespace Engine
{
	class LandscapeComponent :
		public TerrainComponent
	{
	private:
		ProceduralTerrainProgram* fillProgram;
		ProceduralTerrainProgram* wireProgram;
		ProceduralTerrainProgram* pointProgram;
		ProceduralTerrainProgram* shadowProgram;

		ProceduralTerrainProgram* activeProgram;

		Object* landscapeTile;

	public:
		LandscapeComponent();

		virtual void Initialize();
		virtual uint32 GetRenderRadius();
		virtual void PreRenderComponent();
		virtual void RenderComponent(int i, int j, Engine::Camera* camera);
		virtual void RenderShadow(const glm::mat4& projection, int i, int j, Engine::Camera* cam);
		virtual void NotifyRenderModeChange(Engine::RenderMode mode);

		virtual inline Program* GetActiveProgram() { return  activeProgram; }

		virtual inline Program* GetShadowMapProgram() { return  shadowProgram; }
	};
}


