#pragma once
#include "PostProcessProgram.h"

namespace Engine
{
    class SSReflectionProgram :
        public PostProcessProgram
    {
	public:
		static const std::string PROGRAM_NAME;
	private:
		uint32 uProjMat;
		uint32 uPosBuffer;
		uint32 uNormalBuffer;
		uint32 uDepthBuffer;
		uint32 uSpecularBuffer;
		uint32 uLightDir;
	public:
		SSReflectionProgram(std::string name, ullong params);
		SSReflectionProgram(const SSReflectionProgram& other);

		void ConfigureProgram();
		void OnRenderObject(const Object* obj, Camera* camera);
    };

	class SSReflectionProgramFactory :
		public ProgramFactory
	{
	public:
		Program* CreateProgram(ullong params);
	};
}


