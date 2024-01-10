#include "Scene.h"
#include "../Config/WorldConfig.h"
#include "../Render/Manager/LightBufferManager.h"

Engine::Scene::Scene()
{
	terrain = new Terrain(Setting::worldTileScale, Setting::worldRenderRadius);
	skyBox = new Engine::SkyBox();
}

Engine::Scene::~Scene()
{
	if(terrain != nullptr)
		delete terrain;

	if (skyBox != nullptr)
		delete skyBox;
}

void Engine::Scene::Initialize()
{
	Engine::GPU::LightBufferManager::getInstance()->OnSceneStart();
}

Engine::SceneManager::SceneManager()
{
	activeScene = nullptr;
}

void Engine::SceneManager::RegisterScene(string sceneName, Scene* scene)
{
	scenes.emplace(sceneName, scene);
}

Engine::SceneManager::~SceneManager()
{
	for (auto item : scenes)
	{
		delete item.second;
	}
}