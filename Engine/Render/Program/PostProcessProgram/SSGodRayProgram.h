#pragma once
#include "PostProcessProgram.h"

namespace Engine
{
	class SSGodRayProgram :
		public PostProcessProgram
	{
	public:
		static const std::string PROGRAM_NAME;

	private:
		uint32 uLightScreenPos;

		uint32 uWeight;
		uint32 uDensity;
		uint32 uDecay;
		uint32 uExposure;
		uint32 uOnlyPass;

		uint32 uAlpha;

	public:
		SSGodRayProgram(std::string name, unsigned long long params);
		SSGodRayProgram(const SSGodRayProgram& other);

		void ConfigureProgram();
		void OnRenderObject(const Object* obj, Camera* camera);
	};

	class SSGodRayProgramFactory : public ProgramFactory
	{
	public:
		Program* CreateProgram(ullong params);
	};

}


