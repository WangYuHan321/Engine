#include "Renderer.h"
#include "../Scene/Scene.h"


// ==================================================================

unsigned int Engine::ScreenManager::SCREEN_HEIGHT = 500;
unsigned int Engine::ScreenManager::SCREEN_WIDTH = 500;

unsigned int Engine::ScreenManager::REAL_SCREEN_HEIGHT = 500;
unsigned int Engine::ScreenManager::REAL_SCREEN_WIDTH = 500;

Engine::Renderer::Renderer()
{
	activeCam = 0;
}

Engine::Renderer::~Renderer()
{
}

void Engine::Renderer::Initialize()
{
	RenderFromCamera(Engine::SceneManager::getInstance()->GetActiveScene()->GetCamera());
}

void Engine::Renderer::RenderFromCamera(Engine::Camera* cam)
{
	activeCam = cam;
}


Engine::RenderManager::RenderManager()
{
	activeRender = 0;
}

Engine::RenderManager::~RenderManager()
{
	if (activeRender != NULL)
	{
		delete activeRender;
	}
}

void Engine::RenderManager::DoRender()
{
	activeRender->DoRender();
}

void Engine::RenderManager::SetRenderer(Engine::Renderer* renderer)
{
	if (activeRender != NULL)
	{
		delete activeRender;
	}

	if (Engine::SceneManager::getInstance()->GetActiveScene() == NULL)
	{
		std::cerr << "RenderManager: No active scene found" << std::endl;
		exit(-1);
	}

	activeRender = renderer;
	activeRender->Initialize();
}

Engine::Renderer* Engine::RenderManager::GetRenderer()
{
	return activeRender;
}

void Engine::RenderManager::DoReSize(uint32 w, uint32 h)
{
	Engine::ScreenManager::SCREEN_WIDTH = Engine::ScreenManager::REAL_SCREEN_WIDTH = w;
	Engine::ScreenManager::SCREEN_HEIGHT = Engine::ScreenManager::REAL_SCREEN_HEIGHT = h;
	activeRender->OnReSize(w, h);
}