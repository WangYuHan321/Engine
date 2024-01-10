#include "SSReflectionProgram.h"
#include "../../Render/Renderer.h"
#include "../../Config/WorldConfig.h"
#include "../../Render/DeferredRender/DeferredRenderer.h"

const Engine::string Engine::SSReflectionProgram::PROGRAM_NAME = "SSReflectionProgram";

Engine::SSReflectionProgram::SSReflectionProgram(std::string name, ullong params):
	Engine::PostProcessProgram(name, params)
{
	fShaderFile = "shader/postprocess/SSReflections.frag";
}

Engine::SSReflectionProgram::SSReflectionProgram(const SSReflectionProgram& other):
	Engine::PostProcessProgram(other)
{
	uProjMat = other.uProjMat;
	uPosBuffer = other.uPosBuffer;
	uNormalBuffer = other.uNormalBuffer;
	uDepthBuffer = other.uDepthBuffer;
	uSpecularBuffer = other.uSpecularBuffer;
	uLightDir = other.uLightDir;
}

void Engine::SSReflectionProgram::ConfigureProgram()
{
	Engine::PostProcessProgram::ConfigureProgram();

	uProjMat = glGetUniformLocation(shaderPtr->GetShaderID(), "projMat");
	uPosBuffer = glGetUniformLocation(shaderPtr->GetShaderID(), "posBuffer");
	uNormalBuffer = glGetUniformLocation(shaderPtr->GetShaderID(), "normalBuffer");
	uDepthBuffer = glGetUniformLocation(shaderPtr->GetShaderID(), "depthBuffer");
	uSpecularBuffer = glGetUniformLocation(shaderPtr->GetShaderID(), "specularBuffer");
	uLightDir = glGetUniformLocation(shaderPtr->GetShaderID(), "lightDirection");
}

void Engine::SSReflectionProgram::OnRenderObject(const Object* obj, Camera* camera)
{
	Engine::PostProcessProgram::OnRenderObject(obj, camera);

	Engine::DeferredRenderer* deferred = static_cast<Engine::DeferredRenderer*>(Engine::RenderManager::getInstance()->GetRenderer());

	glUniformMatrix4fv(uProjMat, 1, GL_FALSE, &(camera->getProjectionMatrix()[0][0]));
	glUniform1i(uPosBuffer, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, deferred->GetGBufferPos()->GetTexture()->GetTextureID());
	glUniform1i(uNormalBuffer, 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, deferred->GetGBufferNormal()->GetTexture()->GetTextureID());
	glUniform1i(uDepthBuffer, 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, deferred->GetGBufferDepth()->GetTexture()->GetTextureID());
	glUniform1i(uSpecularBuffer, 4);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, deferred->GetGBufferSpecular()->GetTexture()->GetTextureID());

	glm::vec3 normalDir = glm::normalize(Engine::Setting::lightDirection);
	glUniform3fv(uLightDir, 1, &normalDir[0]);

}


Engine::Program* Engine::SSReflectionProgramFactory::CreateProgram(ullong params)
{
	Engine::SSReflectionProgram* prog = new Engine::SSReflectionProgram(Engine::SSReflectionProgram::PROGRAM_NAME, params);
	prog->Initialize();
	return prog;
}