#include "SSGodRayProgram.h"
#include "../../Config/WorldConfig.h"

const Engine::string Engine::SSGodRayProgram::PROGRAM_NAME = "SSGodRayProgram";

Engine::SSGodRayProgram::SSGodRayProgram(std::string name, unsigned long long params):
	Engine::PostProcessProgram(name, params)
{
	fShaderFile = "shader/postprocess/SSGodRays.frag";
}

Engine::SSGodRayProgram::SSGodRayProgram(const SSGodRayProgram& other):
	Engine::PostProcessProgram(other)
{
	uLightScreenPos = other.uLightScreenPos;

	uWeight = other.uWeight;
	uDensity = other.uDensity;
	uDecay = other.uDecay;
	uExposure = other.uExposure;
	uOnlyPass = other.uOnlyPass;

	uAlpha = other.uAlpha;
}

void Engine::SSGodRayProgram::ConfigureProgram()
{
	Engine::PostProcessProgram::ConfigureProgram();

	uLightScreenPos = glGetUniformLocation(shaderPtr->GetShaderID(), "lightScreenPos");
	uWeight = glGetUniformLocation(shaderPtr->GetShaderID(), "weight");
	uExposure = glGetUniformLocation(shaderPtr->GetShaderID(), "exposure");
	uDensity = glGetUniformLocation(shaderPtr->GetShaderID(), "density");
	uDecay = glGetUniformLocation(shaderPtr->GetShaderID(), "decay");
	uOnlyPass = glGetUniformLocation(shaderPtr->GetShaderID(), "onlyPass");
	uAlpha = glGetUniformLocation(shaderPtr->GetShaderID(), "alpha");
}

void Engine::SSGodRayProgram::OnRenderObject(const Object* obj, Camera* camera)
{
	Engine::PostProcessProgram::OnRenderObject(obj, camera);

	// Project light position on screen
	glm::vec4 lightDir = glm::vec4(-camera->getPosition() + Engine::Setting::lightDirection * 20.0f, 1.0);
	lightDir = camera->getProjectionMatrix() * camera->getViewMatrix() * lightDir;
	lightDir /= lightDir.w;
	lightDir = lightDir * 0.5f + 0.5f;

	// Its a screen space effect based on sun position on screen. When its offscreen, it produces artifacts
	// make sure we only apply when its ok (with a little margin)
	bool outScreen = lightDir.z > 1.f || lightDir.x < -2.f || lightDir.x > 3.f || lightDir.y < -2.f || lightDir.y > 3.f;

	glUniform1i(uOnlyPass, outScreen);
	glUniform2fv(uLightScreenPos, 1, &lightDir[0]);

	// Send tweakable data
	glUniform1f(uWeight, Engine::Setting::godRaysWeight);
	glUniform1f(uExposure, Engine::Setting::godRaysExposure);
	glUniform1f(uDensity, Engine::Setting::godRaysDensity);
	glUniform1f(uDecay, Engine::Setting::godRaysDecay);
}

Engine::Program* Engine::SSGodRayProgramFactory::CreateProgram(ullong params)
{
	Engine::SSGodRayProgram* prog = new Engine::SSGodRayProgram(Engine::SSGodRayProgram::PROGRAM_NAME, params);
	prog->Initialize();
	return prog;
}

