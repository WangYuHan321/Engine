#include "RenderableNotifier.h"
Engine::RenderableNotifier::RenderableNotifier()
{
	previousDrawingMethod = Engine::Setting::drawingMethod;
	previousLightColor = Engine::Setting::lightColor;
	previousLightDir = Engine::Setting::lightDirection;
}

void Engine::RenderableNotifier::RegisterRenderable(Engine::IRenderable* renderable)
{
	renderables.push_back(renderable);
}

void Engine::RenderableNotifier::RegisterLight(Engine::Light* light)
{
	lights.push_back(light);
}

void Engine::RenderableNotifier::CheckUpdateConfig()
{
	if (previousDrawingMethod != Engine::Setting::drawingMethod)
	{
		previousDrawingMethod = Engine::Setting::drawingMethod;
		RenderMode mode = static_cast<RenderMode>(previousDrawingMethod);
		for (auto render : renderables)
		{
			render->NotifyRenderModeUpdate(mode);
		}
	}

	if (previousLightColor != Engine::Setting::lightColor
		|| previousLightDir != Engine::Setting::lightDirection)
	{
		previousLightColor = Engine::Setting::lightColor;
		previousLightDir = Engine::Setting::lightDirection;

		for (auto light : lights)
		{
			light->Translate(previousLightDir);
			light->SetColor(previousLightColor);
		}
	}
}