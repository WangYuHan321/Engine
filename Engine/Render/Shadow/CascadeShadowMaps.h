#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "../../Util/Singleton.h"
#include "ShadowCaster.h"
#include "../Texture/TextureInstance.h"
#include "../DeferredRender/DeferredRenderObject.h"

namespace Engine
{
	class CascadeShadowMaps : public CSingleton<CascadeShadowMaps>
	{
	private:

		typedef struct ShadowMap
		{
			glm::mat4 proj;
			glm::mat4 depth;
			DeferredRenderObject* rtt;
			TextureInstance* shadowMap;
		} ShadowMap;

	private:

		ShadowMap shadowMap[2];

		int currentLevel;

		glm::mat4 biasMatrix; //?

		int previousFrameBuffer;

		std::vector<ShadowCaster* > shadowCasters;

	public:

		void Init();
		void InitializeFrame(Engine::Camera* cam);
		void RenderShadows(Camera* cam);
		void InitializeShadowMap(int level, float minX, float maxX, float miny, float maxY, float minz, float maxZ);

		void BeginShadowRender(int level);
		void EndShadowRender();
		
		uint32 GetCascadeLevel()
		{
			return 2;
		}

		const glm::mat4& GetDepthMatrix0()
		{
			return shadowMap[0].depth;
		}

		const glm::mat4& GetDepthMatrix1()
		{
			return shadowMap[1].depth;
		}

		const glm::mat4& GetShadowProjectionMat()
		{
			return shadowMap[currentLevel].depth;
		}

		const Engine::TextureInstance* GetDepthTexture1()
		{
			return shadowMap[1].shadowMap;
		}

		const Engine::TextureInstance* GetDepthTexture0()
		{
			return shadowMap[0].shadowMap;
		}

		void RegisterShadowCaster(ShadowCaster* caster);
	};
}


