#include "SkyProgram.h"
#include "../../Config/WorldConfig.h"

Engine::string Engine::SkyProgram::PROGRAM_NAME = "SkyProgram";

Engine::SkyProgram::SkyProgram(string name, ullong params):
	Engine::Program(name, params)
{
	vShaderFile = "shader/sky/sky.vert";
	fShaderFile = "shader/sky/sky.frag";
}

Engine::SkyProgram::SkyProgram(const SkyProgram& other):
	Engine::Program(other)
{
	inPos = other.inPos;
	uProjMatrix = other.uProjMatrix;
	uLightDir = other.uLightDir;
	uLightColor = other.uLightColor;
	uColorFactor = other.uColorFactor;
	uSkyZenitColor = other.uSkyZenitColor;
	uSkyHorizonColor = other.uSkyHorizonColor;
}

void Engine::SkyProgram::ConfigureProgram()
{
	uProjMatrix = glGetUniformLocation(shaderPtr->GetShaderID(), "proj");
	uLightDir = glGetUniformLocation(shaderPtr->GetShaderID(), "lightDir");
	uLightColor = glGetUniformLocation(shaderPtr->GetShaderID(), "lightColor");
	uColorFactor = glGetUniformLocation(shaderPtr->GetShaderID(), "colorFactor");

	uSkyZenitColor = glGetUniformLocation(shaderPtr->GetShaderID(), "zenitColor");
	uSkyHorizonColor = glGetUniformLocation(shaderPtr->GetShaderID(), "horizonColor");

	inPos = glGetAttribLocation(shaderPtr->GetShaderID(), "inPos");
}

void Engine::SkyProgram::ConfigureMeshBuffer(Mesh* mesh)
{
	mesh->use();

	if (inPos != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vboVertices);
		glVertexAttribPointer(inPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inPos);
	}
}

void Engine::SkyProgram::OnRenderObject(const Object* obj, Camera* camera)
{
	glm::mat4 modelViewProj = camera->getProjectionMatrix() * camera->getViewMatrix() * obj->GetModelMatrix();
	glUniformMatrix4fv(uProjMatrix, 1, GL_FALSE, &(modelViewProj[0][0]));

	glm::vec3 direction = -glm::normalize(Engine::Setting::lightDirection);
	glUniform3fv(uLightDir, 1, &direction[0]);

	glUniform3fv(uLightColor, 1, &Engine::Setting::realLightColor[0]);
	glUniform1f(uColorFactor, Engine::Setting::lightFactor);

	glUniform3fv(uSkyZenitColor, 1, &Engine::Setting::skyZenitColor[0]);
	glUniform3fv(uSkyHorizonColor, 1, &Engine::Setting::skyHorizonColor[0]);
}

Engine::Program* Engine::SkyProgramFactory::CreateProgram(ullong parameters)
{
	Engine::SkyProgram* program = new Engine::SkyProgram(Engine::SkyProgram::PROGRAM_NAME, parameters);
	program->Initialize();
	return program;
}
