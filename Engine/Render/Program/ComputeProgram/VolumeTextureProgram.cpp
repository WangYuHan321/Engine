#include "VolumeTextureProgram.h"

Engine::VolumeTextureProgram::VolumeTextureProgram(string computerShaderFile):
	Engine::ComputeProgram(computerShaderFile)
{
}

Engine::VolumeTextureProgram::VolumeTextureProgram(const VolumeTextureProgram& program):
	Engine::ComputeProgram(program)
{
}

void Engine::VolumeTextureProgram::ConfigureProgram()
{
	uOutput = glGetUniformLocation(shaderPtr->GetShaderID(), "outVolTex");
}

void Engine::VolumeTextureProgram::BindOutput(const Engine::TextureInstance* ti)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, ti->GetTexture()->GetTextureID());
	glBindImageTexture(0, ti->GetTexture()->GetTextureID(), 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA8);
	glUniform1i(uOutput, 0);
}


//Engine::Program* Engine::VolumeTextureProgramFactory::CreateProgram(ullong paramters)
//{
//	Engine::VolumeTextureProgram* program = new Engine::VolumeTextureProgram(Engine::VolumeTextureProgram::PROGRAM_NAME, paramters);
//	program->Initialize();
//	return program;
//}
//
//Engine::Program* Engine::VolumeTextureProgramFactory::CreateProgram(string name, ullong paramters)
//{
//	Engine::VolumeTextureProgram* program = new Engine::VolumeTextureProgram(name, paramters);
//	program->Initialize();
//	return program;
//}