#pragma once
#include "PostProcessProgram.h"

namespace Engine
{
	class CloudFilterProgram :
		public PostProcessProgram
	{
	public:
		static string PROGRAM_NAME;

	private:

		uint32 uTexelSize;
		uint32 uLightColor;

		uint32 uRepro1;
		uint32 uRepro2;
		uint32 uRepro3;
		uint32 uRepro4;

		uint32 uVel1, uVel2, uVel3, uVel4;

		uint32 uFrameIter;
		uint32 uOldProjView;

		glm::mat4 oldView;

	public:

		CloudFilterProgram(string name, ullong params);
		CloudFilterProgram(const CloudFilterProgram& other);
		~CloudFilterProgram();

		virtual void ConfigureProgram();
		virtual void OnRenderObject(const Object* obj, Camera* camera);

		void SetBufferInput(TextureInstance** buffers);
		void SetVelocityInput(TextureInstance** velocities);

	};

	class CloudFilterProgramFactory : public ProgramFactory
	{
	protected:
		Program* CreateProgram(unsigned long long parameters);
	};
}


