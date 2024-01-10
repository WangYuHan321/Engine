#include "Program.h"
Engine::Program::Program(string name, ullong params)
	:
	name(name),
	paramters(params)
{
}

Engine::Program::Program(const Program& other)
{
	name = other.name;
	paramters = other.paramters;
	shaderPtr = other.shaderPtr;

	vShaderFile = other.vShaderFile;
	fShaderFile = other.fShaderFile;
	//cShaderFile = other.cShaderFile;
	gShaderFile = other.gShaderFile;
}

Engine::Program::~Program()
{
}

Engine::uint32 Engine::Program::GetProgramID() const
{
	return shaderPtr->GetShaderID();
}

void Engine::Program::Initialize()
{
	if (vShaderFile.empty() || fShaderFile.empty())
	{
		std::cout << name << ": Failed to initialize - no shader files specified" << std::endl;
		return;
	}

	shaderPtr = new Shader(vShaderFile, fShaderFile);

	ConfigureProgram();
}

void Engine::Program::Use()
{
	shaderPtr->Use();
}

void Engine::Program::ApplyGlobalUniforms()
{

}

void Engine::Program::Destory()
{

}


Engine::Program* Engine::ProgramFactory::InstantiateProgram(unsigned long long parameters)
{
	std::map<unsigned long long, Program*>::iterator it = cache.find(parameters);
	if (it != cache.end())
	{
		return it->second;
	}
	else
	{
		Engine::Program* program = CreateProgram(parameters);
		cache[parameters] = program;
		return program;
	}
}

void Engine::ProgramFactory::Clean()
{
	std::map<unsigned long long, Program*>::iterator it = cache.begin();
	while (it != cache.end())
	{
		it->second->Destory();
		delete it->second;
		it++;
	}

	cache.clear();
}