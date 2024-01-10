#include "PointLight.h"

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

Engine::PointLight::PointLight(string name):
	Light(name)
{
}

Engine::PointLight::~PointLight()
{
}

void Engine::PointLight::SetPosition(const glm::vec3& pos)
{
	
}

void Engine::PointLight::SetAttenuation(const glm::vec3& att)
{

}

void Engine::PointLight::SetColor(const glm::vec3& color)
{

}

void Engine::PointLight::SetKa(float a)
{

}

void Engine::PointLight::SetKd(float d)
{

}

void Engine::PointLight::SetKs(float s)
{

}

void Engine::PointLight::Translate(const glm::vec3& translation)
{
	modelMatrix = glm::translate(glm::mat4(1.0f), translation);
}