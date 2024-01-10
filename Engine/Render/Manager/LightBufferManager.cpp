#include "LightBufferManager.h"

Engine::GPU::LightBufferManager::LightBufferManager()
{
}

Engine::GPU::LightBufferManager::~LightBufferManager()
{
	Clean();
}

void Engine::GPU::LightBufferManager::OnSceneStart()
{
	Clean();

	Engine::Scene* scene = Engine::SceneManager::getInstance()->GetActiveScene();
	if (scene != nullptr)
	{
		InitializeBuffers(scene);
	}

}

void Engine::GPU::LightBufferManager::EnableDirectionalLightBuffer()
{
	glBindBuffer(GL_UNIFORM_BUFFER, gboDL);
}
void Engine::GPU::LightBufferManager::EnablePointLightBuffer()
{
	glBindBuffer(GL_UNIFORM_BUFFER, gboPL);
}
void Engine::GPU::LightBufferManager::EnableSpotLightBuffer()
{
	glBindBuffer(GL_UNIFORM_BUFFER, gboSL);
}
void Engine::GPU::LightBufferManager::EnableDirectionalLightBufferAtIndex(uint32 index)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, index, gboDL);
}
void Engine::GPU::LightBufferManager::EnablePointLightBufferAtIndex(uint32 index)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, index, gboPL);
}
void Engine::GPU::LightBufferManager::EnableSpotLightBufferAtIndex(uint32 index)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, index, gboSL);
}

void Engine::GPU::LightBufferManager::UpdateDirectionalLight(DirectionalLight* dl, bool onlyViewDependent)
{
	size_t structSize = sizeof(Engine::DirectionalLightData);
	if (onlyViewDependent)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, structSize * dl->GetBufferIndex(), sizeof(float) * 4, dl->GetData().direction);
	}
	else
	{
		glBufferSubData(GL_UNIFORM_BUFFER, structSize * dl->GetBufferIndex(), structSize, &dl->GetData());
	}
}
void Engine::GPU::LightBufferManager::UpdateDirectionalLight(PointLight* pl, bool onlyViewDependent)
{

}
void Engine::GPU::LightBufferManager::UpdateDirectionalLight(SpotLight* sl, bool onlyViewDependent)
{

}

void Engine::GPU::LightBufferManager::Clean()
{
	if (gboDL != -1)
		glDeleteBuffers(1, &gboDL);

	if (gboPL != -1)
		glDeleteBuffers(1, &gboPL);

	if (gboSL != -1)
		glDeleteBuffers(1, &gboSL);
}

void Engine::GPU::LightBufferManager::InitializeBuffers(Engine::Scene* scene)
{
	Engine::DirectionalLight* dl = scene->GetDirectionLight();
	const std::map<std::string, Engine::PointLight*>& pl = scene->GetPointLights();
	const std::map<std::string, Engine::SpotLight*>& sl = scene->GetSpotLights();

	if (dl != nullptr)
	{
		glGenBuffers(1, &gboDL);
		glBindBuffer(GL_UNIFORM_BUFFER, gboDL);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(Engine::DirectionalLightData), &dl->GetData(), GL_DYNAMIC_DRAW);
	}
	
	if (pl.size() > 0)
	{
	}

	if (sl.size() > 0)
	{
	}
}
