#include "SideBySideRenderer.h"

Engine::SideBySideRenderer::SideBySideRenderer()
{
	//leftRenderer = new Engine::DeferredRenderer();
	//rightRenderer = new Engine::DeferredRenderer();

}

Engine::SideBySideRenderer::~SideBySideRenderer()
{
	if (leftRenderer != nullptr)
		delete leftRenderer;
	if (rightRenderer != nullptr)
		delete rightRenderer;
}

void Engine::SideBySideRenderer::AddLeftPostProcess()
{
	
}

void Engine::SideBySideRenderer::AddRightPostProcess()
{

}

void Engine::SideBySideRenderer::Initialize()
{

}

void Engine::SideBySideRenderer::DoRender()
{

}

void Engine::SideBySideRenderer::OnReSize(uint32 w, uint32 h)
{

}