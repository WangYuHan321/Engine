#include "ComputeProgram.h"
Engine::ComputeProgram::ComputeProgram(std::string shaderFile):
	computeShaderFile(shaderFile)
{
}

Engine::ComputeProgram::ComputeProgram(const ComputeProgram& other)
{
	shaderPtr = other.shaderPtr;
	computeShaderFile = other.computeShaderFile;
}

Engine::uint32 Engine::ComputeProgram::GetProgramID()
{
	return shaderPtr->GetShaderID();
}

void Engine::ComputeProgram::Use()
{
	glUseProgram(shaderPtr->GetShaderID());
}

void Engine::ComputeProgram::Initialize()
{
	shaderPtr = new Shader(computeShaderFile);
	ConfigureProgram();
}

void Engine::ComputeProgram::Dispatch(unsigned int xDim, unsigned int yDim, unsigned int zDim, unsigned int barrier)
{
	glDispatchCompute(xDim, yDim, zDim);
	glMemoryBarrier(barrier);
}

void Engine::ComputeProgram::Destroy()
{
	shaderPtr->Clean();
}