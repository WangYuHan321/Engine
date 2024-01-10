#pragma once

#include "../../Util/common.h"
#include "../IRenderable.h"
#include "../Shadow/ShadowCaster.h"
#include "Component/TerrainComponent.h"

namespace Engine
{
	class Terrain : public IRenderable, public ShadowCaster
	{
	private:
		float tileWidth;
		uint32 renderRadius;

		std::vector<TerrainComponent*> renderableComponent;
		std::vector<TerrainComponent*> shadowableComponent;

	public:
		Terrain();
		Terrain(float tileWidth, float renderRadius);
		~Terrain();

		void RegisterComponent(TerrainComponent* comp);

		void Render(Camera* cam);
		void RenderShadow(Camera* camera, const glm::mat4& projectionMatrix);

		void NotifyRenderModeUpdate(RenderMode mode);

		inline float GetTileScale() { return tileWidth; };
		inline uint32 GetRenderRadius() { return renderRadius; };

	private:

		void Initialize();
		//void CreateTileMesh();

		void RenderTiledComponent(TerrainComponent* component, Camera* cam);
		void RenderTiledComponentShadow(TerrainComponent* component, Camera* cam, const glm::mat4& proj);
	};
}


