#pragma once
#include "PostProcessProgram.h"
#include "../../Light/DirectionalLight.h"

namespace Engine
{
	class DeferredShadingProgram :
		public PostProcessProgram
	{
	public:
		static string PROGRAM_NAME;
	private:
		//dirtection light data buffer
		uint32 uDLBuffer;
		
		//point light buffer
		uint32 uPLBuffer;

		//spoint light buffer
		uint32 uSLBuffer;

		uint32 uSkyZenitColor;
		uint32 uSkyHorizonColor;

		uint32 uColorFactor;

	public:
		DeferredShadingProgram(string name, ullong paramters);
		DeferredShadingProgram(const DeferredShadingProgram& other);

		void OnRenderObject(const Object* obj, Camera* cam);
		void ConfigureProgram();
	private:
		void ProcessDirectionalLights(DirectionalLight* dl, const glm::mat4& view);
	};

	class DeferredShadingProgramFactory : public ProgramFactory
	{
	protected:
		Program* CreateProgram(ullong paramters);
	};
}


