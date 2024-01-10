#pragma once
#include "../Program.h"

namespace Engine
{
	class CloudShadowProgram :
		public Program
	{
	public :
		static string PROGRAM_NAME;

	private:

		uint32 uInPos;
		uint32 uInUV;
		uint32 uLightProjMat;
		uint32 uWeather;
		uint32 uCoverageMult;

	public:

		CloudShadowProgram(string name, ullong paramter);
		CloudShadowProgram(const CloudShadowProgram& other);


		void ConfigureMeshBuffer(Mesh* mesh);
		void ConfigureProgram();
		void OnRenderObject(const Object* obj, Camera* cam);
		void SetUniformLightProjMatrix(const glm::mat4& proj);

	};

	class CloudShadowProgramFactory : public ProgramFactory
	{
	public:
		Program* CreateProgram(unsigned long long parameters);
	};
}


