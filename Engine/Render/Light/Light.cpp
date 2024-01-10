#include "Light.h"

Engine::Light::Light(string name)
	:name(name)
{
	SetEnabled(true);
	update = true;

	modelMatrix = glm::mat4(1.0f);
}

Engine::Light::~Light()
{
}

const std::string& Engine::Light::GetName() const
{
	return name;
}

void Engine::Light::SetEnabled(bool val)
{
	enabled = val;
}

const bool Engine::Light::IsEnabled() const
{
	return enabled;
}

const glm::mat4& Engine::Light::GetModelMatrix() const
{
	return modelMatrix;
}

bool Engine::Light::RequiresUpdate()
{
	return update;
}

void Engine::Light::ClearUpdateFlag()
{
	update = false;
}

void Engine::Light::SetBufferIndex(unsigned int bi)
{
	bufferIndex = bi;
}

unsigned int Engine::Light::GetBufferIndex()
{
	return bufferIndex;
}