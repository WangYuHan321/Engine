#pragma once

#include "Program.h"

namespace Engine
{
	class DebugPorgram :
		public Program
	{

	public:
		static string PROGRAM_NAME;
	private:

		uint32 planeVAO;
		uint32 planeVerticesVBO;

		uint32 inPos;
		uint32 inTexCoord;

	protected:
		unsigned int uRenderedTexture;

	public:

		DebugPorgram(std::string name, unsigned long long params);
		DebugPorgram(unsigned long long params);
		DebugPorgram(const DebugPorgram& other);
		~DebugPorgram();

		virtual void ConfigureProgram() override;
		virtual void ConfigureMeshBuffer(Mesh* mesh) override;

		virtual void OnRenderObject(const Object* obj, Camera* cam) override;
	};

	class DebugProgramFactory : public ProgramFactory
	{
	protected:
		virtual Program* CreateProgram(ullong params);
	};

}


