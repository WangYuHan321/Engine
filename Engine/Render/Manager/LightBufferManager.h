#pragma once
#include "../../Scene/Scene.h"

namespace Engine
{
	namespace GPU
	{
		class LightBufferManager : public CSingleton<LightBufferManager>
		{
		private:
			uint32 gboDL;
			uint32 gboPL;
			uint32 gboSL;

		public:
			LightBufferManager();
			~LightBufferManager();

			void OnSceneStart();

			void EnableDirectionalLightBuffer();
			void EnablePointLightBuffer();
			void EnableSpotLightBuffer();
			void EnableDirectionalLightBufferAtIndex(uint32 index);
			void EnablePointLightBufferAtIndex(uint32 index);
			void EnableSpotLightBufferAtIndex(uint32 index);

			void UpdateDirectionalLight(DirectionalLight* dl, bool onlyViewDependent = false);
			void UpdateDirectionalLight(PointLight* pl, bool onlyViewDependent = false);
			void UpdateDirectionalLight(SpotLight* sl, bool onlyViewDependent = false);

			void Clean();

		private:
			void InitializeBuffers(Engine::Scene* scene);

		};
	}

}


