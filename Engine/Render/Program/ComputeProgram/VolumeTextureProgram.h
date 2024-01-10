#pragma once
#include "ComputeProgram.h"

namespace Engine
{
	class VolumeTextureProgram :
		public ComputeProgram
	{
	private:
		uint32 uOutput;
	public:

		VolumeTextureProgram(string computerShaderFile);
		VolumeTextureProgram(const VolumeTextureProgram& program);

		void ConfigureProgram();
		void BindOutput(const Engine::TextureInstance* ti);
	};

	//class VolumeTextureProgramFactory : public ProgramFactory
	//{
	//protected:
	//	virtual Program* CreateProgram(ullong paramters) override;
	//	Program* CreateProgram(string name, ullong paramters);
	//};
}



