#pragma once

#include "../Shadow/ShadowCaster.h"
#include "../DeferredRender/DeferredRenderObject.h"
#include "../Program/PostProcessProgram/CloudFilterProgram.h"
#include "../Program/CloudProgram/VolumetricCloudProgram.h"

namespace Engine
{
	namespace CloudSystem
	{
		class VolumetricClouds :
			public ShadowCaster
		{
		private:
			// Volume rendering clouds program
			VolumetricCloudProgram* volumeCloudProgram;

			CloudFilterProgram* filterProgram;

			Mesh* renderPlane;;
			Object* skyPlane;

			DeferredRenderObject* reprojectionBuffer[4];

			TextureInstance* reproBuffer[4];
		public:
			VolumetricClouds();

			void Render(Camera* cam);
			void RenderShadow(Camera* camera, const glm::mat4& projectionMatrix);
		};
	}

}


