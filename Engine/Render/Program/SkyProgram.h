#pragma once
#include "Program.h"

namespace Engine
{
	class SkyProgram :
		public Program
	{
	public:
		static string PROGRAM_NAME;

	private:
		uint32 uProjMatrix;
		uint32 uLightDir;
		uint32 uLightColor;
		uint32 uColorFactor;

		uint32 uSkyZenitColor;
		uint32 uSkyHorizonColor;

		uint32 inPos;

	public:

		SkyProgram(string name, ullong params);
		SkyProgram(const SkyProgram& other);

		void ConfigureProgram();
		void ConfigureMeshBuffer(Mesh* mesh);
		void OnRenderObject(const Object* obj, Camera* camera);

	};


	class SkyProgramFactory : public ProgramFactory
	{
		protected:
			Program* CreateProgram(ullong parameters);
	};
}


