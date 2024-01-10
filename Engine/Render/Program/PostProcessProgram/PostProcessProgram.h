#pragma once
#include "../Program.h"
namespace Engine
{
	class PostProcessProgram :
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
		unsigned int uRenderedTextures[9];

	public:

		PostProcessProgram(std::string name, unsigned long long params);
		PostProcessProgram(unsigned long long params);
		PostProcessProgram(const PostProcessProgram& other);
		~PostProcessProgram();

		virtual void ConfigureProgram() override;
		virtual void ConfigureMeshBuffer(Mesh* mesh) override;

		virtual void OnRenderObject(const Object* obj, Camera* cam) override;
	};

	class PostProcessProgramFactory : public ProgramFactory
	{
	protected:
		virtual Program* CreateProgram(ullong params);
	};
}


