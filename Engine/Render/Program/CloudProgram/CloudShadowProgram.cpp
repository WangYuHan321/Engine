#include "CloudShadowProgram.h"

Engine::string Engine::CloudShadowProgram::PROGRAM_NAME = "CloudShadowProgram";

Engine::CloudShadowProgram::CloudShadowProgram(string name, ullong paramter):
	Engine::Program(name, paramter)
{
	vShaderFile = "shader/clouds/cloudshadow.vert";
	fShaderFile = "shader/clouds/cloudshadow.frag";
}

Engine::CloudShadowProgram::CloudShadowProgram(const CloudShadowProgram& other):
	Engine::Program(other)
{
	uInPos = other.uInPos;
	uInUV = other.uInUV;
	uLightProjMat = other.uLightProjMat;
	uWeather = other.uWeather;
	uCoverageMult = other.uCoverageMult;
}

void Engine::CloudShadowProgram::ConfigureMeshBuffer(Mesh* mesh)
{

}

void Engine::CloudShadowProgram::ConfigureProgram()
{

}

void Engine::CloudShadowProgram::OnRenderObject(const Object* obj, Camera* cam)
{

}

void Engine::CloudShadowProgram::SetUniformLightProjMatrix(const glm::mat4& proj)
{

}

Engine::Program* Engine::CloudShadowProgramFactory::CreateProgram(unsigned long long parameters)
{
	Engine::CloudShadowProgram* p = new Engine::CloudShadowProgram(Engine::CloudShadowProgram::PROGRAM_NAME, parameters);
	p->Initialize();
	return p;
}