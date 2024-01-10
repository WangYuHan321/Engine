#include "CloudFilterProgram.h"
#include "../../Render/Renderer.h"
#include "../../Util/Time.h"
#include "../../Config/WorldConfig.h"

Engine::string Engine::CloudFilterProgram::PROGRAM_NAME = "CloudFilterProgram";

Engine::CloudFilterProgram::CloudFilterProgram(string name, ullong params):
	Engine::PostProcessProgram(name, params)
{
	fShaderFile = "shader/clouds/cloudfilter.frag";
	oldView = glm::mat4(1.0f);
}

Engine::CloudFilterProgram::CloudFilterProgram(const CloudFilterProgram& other):
	Engine::PostProcessProgram(other)
{
	uTexelSize = other.uTexelSize;
	uLightColor = other.uLightColor;
	uRepro1 = other.uRepro1;
	uRepro2 = other.uRepro2;
	uRepro3 = other.uRepro3;
	uRepro4 = other.uRepro4;
	uOldProjView = other.uOldProjView;
}

Engine::CloudFilterProgram::~CloudFilterProgram()
{
}

void Engine::CloudFilterProgram::ConfigureProgram()
{
	Engine::PostProcessProgram::ConfigureProgram();

	uTexelSize = glGetUniformLocation(shaderPtr->GetShaderID(), "texelSize");
	uLightColor = glGetUniformLocation(shaderPtr->GetShaderID(), "realLightColor");
	uRepro1 = glGetUniformLocation(shaderPtr->GetShaderID(), "repro1");
	uRepro2 = glGetUniformLocation(shaderPtr->GetShaderID(), "repro2");
	uRepro3 = glGetUniformLocation(shaderPtr->GetShaderID(), "repro3");
	uRepro4 = glGetUniformLocation(shaderPtr->GetShaderID(), "repro4");
	uVel1 = glGetUniformLocation(shaderPtr->GetShaderID(), "vel1");
	uVel2 = glGetUniformLocation(shaderPtr->GetShaderID(), "vel2");
	uVel3 = glGetUniformLocation(shaderPtr->GetShaderID(), "vel3");
	uVel4 = glGetUniformLocation(shaderPtr->GetShaderID(), "vel4");
	uOldProjView = glGetUniformLocation(shaderPtr->GetShaderID(), "oldProjView");
	uFrameIter = glGetUniformLocation(shaderPtr->GetShaderID(), "frameIter");
}

void Engine::CloudFilterProgram::OnRenderObject(const Object* obj, Camera* camera)
{
	glUniform2f(uTexelSize, 1.0f / ((float)ScreenManager::SCREEN_WIDTH), 1.0f / ((float)ScreenManager::SCREEN_HEIGHT));
	glUniform3fv(uLightColor, 1, &Engine::Setting::realLightColor[0]);

	glm::mat4 old = camera->getProjectionMatrix() * oldView;
	glUniformMatrix4fv(uOldProjView, 1, GL_FALSE, &(old[0][0]));
	oldView = camera->getViewMatrix();

	glUniform1i(uFrameIter, (GLint)Engine::Time::frame % 2);
}

void Engine::CloudFilterProgram::SetBufferInput(TextureInstance** buffers)
{
	glUniform1i(uRepro1, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, buffers[0]->GetTexture()->GetTextureID());

	glUniform1i(uRepro2, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, buffers[1]->GetTexture()->GetTextureID());
}

void Engine::CloudFilterProgram::SetVelocityInput(TextureInstance** velocities)
{
	glUniform1i(uVel1, 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, velocities[0]->GetTexture()->GetTextureID());

	glUniform1i(uVel2, 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, velocities[1]->GetTexture()->GetTextureID());
}

Engine::Program* Engine::CloudFilterProgramFactory::CreateProgram(ullong parameters)
{
	Engine::CloudFilterProgram* program = new Engine::CloudFilterProgram(Engine::CloudFilterProgram::PROGRAM_NAME, parameters);
	program->Initialize();
	return program;
}
