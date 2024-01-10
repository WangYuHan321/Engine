#include "DirectionalLight.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../../Config/WorldConfig.h"
#include "../Notifier/RenderableNotifier.h"

Engine::DirectionalLight::DirectionalLight(std::string name)
	:Engine::Light(name)
{
	Engine::RenderableNotifier::getInstance()->RegisterLight(this);
	SetKa(0.15f);
	SetKd(1.0f);
	SetKs(1.0f);

	SetBufferIndex(0);
}

Engine::DirectionalLight::~DirectionalLight()
{
}

const glm::vec3 Engine::DirectionalLight::GetDirection() const
{
	return direction;
}

void Engine::DirectionalLight::Translate(const glm::vec3& translation)
{
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, translation);
	modelMatrix[3][3] = 0;

	direction = glm::normalize(glm::vec3(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]));
}

void Engine::DirectionalLight::SetDirection(const glm::vec3& direction)
{
	Translate(direction);
}

void Engine::DirectionalLight::SetColor(const glm::vec3& color)
{
	memcpy(shaderData.color, &color[0], sizeof(float) * 3);
	update = true;
}

void Engine::DirectionalLight::SetKa(float a)
{
	shaderData.kFactors[0] = a;
	update = true;
}

void Engine::DirectionalLight::SetKd(float d)
{
	shaderData.kFactors[1] = d;
	update = true;
}

void Engine::DirectionalLight::SetKs(float s)
{
	shaderData.kFactors[2] = s;
	update = true;
}

Engine::DirectionalLightData& Engine::DirectionalLight::GetData()
{
	return shaderData;
}