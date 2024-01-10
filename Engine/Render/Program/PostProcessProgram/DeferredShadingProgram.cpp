#include "DeferredShadingProgram.h"
#include "../../../Scene/Scene.h"
#include "../../Config/WorldConfig.h"
#include "../../../Render/Manager/LightBufferManager.h"

Engine::string Engine::DeferredShadingProgram::PROGRAM_NAME = "DeferredShadingProgram";

Engine::DeferredShadingProgram::DeferredShadingProgram(string name, ullong paramters):
	Engine::PostProcessProgram(name, paramters)
{
	fShaderFile = "shader/postprocess/deferredshading.frag";
}

Engine::DeferredShadingProgram::DeferredShadingProgram(const DeferredShadingProgram& other):
	Engine::PostProcessProgram(other)
{
	uDLBuffer = other.uDLBuffer;
	uPLBuffer = other.uPLBuffer;
	uSLBuffer = other.uSLBuffer;
	uColorFactor = other.uColorFactor;
}

void Engine::DeferredShadingProgram::OnRenderObject(const Object* obj, Camera* cam)
{
	Engine::PostProcessProgram::OnRenderObject(obj, cam);

	Scene* scenePtr = SceneManager::getInstance()->GetActiveScene();
	if (scenePtr != nullptr)
	{
		Engine::DirectionalLight* dl = scenePtr->GetDirectionLight();
		ProcessDirectionalLights(dl, cam->getViewMatrix());
	}

	glUniform3fv(uSkyZenitColor, 1, &Engine::Setting::skyZenitColor[0]);
	glUniform3fv(uSkyHorizonColor, 1, &Engine::Setting::skyHorizonColor[0]);

	glUniform1f(uColorFactor, Engine::Setting::lightFactor);

}

void Engine::DeferredShadingProgram::ConfigureProgram()
{
	Engine::PostProcessProgram::ConfigureProgram();

	uSkyHorizonColor = glGetUniformLocation(shaderPtr->GetShaderID(), "horizonColor");
	uSkyZenitColor = glGetUniformLocation(shaderPtr->GetShaderID(), "zenitColor");

	uDLBuffer = glGetUniformBlockIndex(shaderPtr->GetShaderID(), "DLBuffer");
	uPLBuffer = glGetUniformBlockIndex(shaderPtr->GetShaderID(), "PLBuffer");
	uSLBuffer = glGetUniformBlockIndex(shaderPtr->GetShaderID(), "SLBuffer");

	uColorFactor = glGetUniformLocation(shaderPtr->GetShaderID(), "colorFactor");
}

void Engine::DeferredShadingProgram::ProcessDirectionalLights(DirectionalLight* dl, const glm::mat4& view)
{
	glm::mat4 resultPos = view * dl->GetModelMatrix();

	glm::vec3 direction(resultPos[3][0], resultPos[3][1], resultPos[3][2]);
	direction = glm::normalize(direction);

	memcpy(dl->GetData().direction, &direction[0], sizeof(float) * 3);

	Engine::GPU::LightBufferManager::getInstance()->EnableDirectionalLightBufferAtIndex(uDLBuffer);
	Engine::GPU::LightBufferManager::getInstance()->UpdateDirectionalLight(dl, dl->RequiresUpdate());
	dl->ClearUpdateFlag();
}

Engine::Program* Engine::DeferredShadingProgramFactory::CreateProgram(ullong paramters)
{
	Engine::DeferredShadingProgram* program = new Engine::DeferredShadingProgram(Engine::DeferredShadingProgram::PROGRAM_NAME, paramters);
	program->Initialize();
	return program;
}