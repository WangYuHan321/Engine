#include "Terrain.h"

#include "../Shadow/CascadeShadowMaps.h"
#include "../Notifier/RenderableNotifier.h"
#include"../Terrain/Component/LandscapeComponent.h"
#include"../Terrain/Component/WaterComponent.h"

Engine::Terrain::Terrain():
	tileWidth(1.0f),
	renderRadius(7)
{
	Initialize();
}

Engine::Terrain::Terrain(float tileWidth_, float renderRadius_):
	tileWidth(tileWidth_),
	renderRadius(renderRadius_)
{
	Initialize();
}

Engine::Terrain::~Terrain()
{
}

void Engine::Terrain::RegisterComponent(TerrainComponent* comp)
{
	if (comp != nullptr)
	{
		renderableComponent.push_back(comp);
		if (comp->CastShadows())
		{
			shadowableComponent.push_back(comp);
		}
	}
}

void Engine::Terrain::Render(Camera* cam)
{
	for (auto& tc : renderableComponent)
	{
		RenderTiledComponent(tc, cam);
	}
}

void Engine::Terrain::RenderShadow(Camera* camera, const glm::mat4& projectionMatrix)
{
	for (auto& sc : shadowableComponent)
	{
		RenderTiledComponentShadow(sc, camera, projectionMatrix);
	}
}

void Engine::Terrain::NotifyRenderModeUpdate(RenderMode mode)
{
	for (auto& v : renderableComponent)
	{
		v->NotifyRenderModeChange(mode);
	}
}

void Engine::Terrain::Initialize()
{
	Engine::RenderableNotifier::getInstance()->RegisterRenderable(this);
	Engine::CascadeShadowMaps::getInstance()->RegisterShadowCaster(this);

	Engine::LandscapeComponent* landscape = new Engine::LandscapeComponent();
	landscape->Init(tileWidth, true);
	RegisterComponent(landscape);

	Engine::WaterComponent* water = new Engine::WaterComponent();
	water->Init(tileWidth, false);
	RegisterComponent(water);
}

//void Engine::Terrain::CreateTileMesh()
//{
//
//}

void Engine::Terrain::RenderTiledComponent(TerrainComponent* component, Camera* cam)
{
	glm::vec3 cameraPosition = cam->getPosition();

	int x = -int((floor(cameraPosition.x)) / tileWidth);
	int y = -int((floor(cameraPosition.z)) / tileWidth);

	unsigned int rr = component->GetRenderRadius();
	int xStart = x - rr;
	int xEnd = x + rr;
	int yStart = y - rr;
	int yEnd = y + rr;

	component->PreRenderComponent();

	Program* prog = component->GetActiveProgram();
	prog->Use();
	prog->ApplyGlobalUniforms();

	// Culling parameters
	glm::vec3 fwd = cam->getForwardVector();
	fwd.y = 0;
	fwd = -glm::normalize(fwd);
	int px = -int(renderRadius), py = px;

	for (int i = xStart; i < xEnd; i++, px++)
	{
		for (int j = yStart; j < yEnd; j++, py++)
		{
			// Culling (skips almost half)
			glm::vec3 test(i - x, 0, j - y);
			if (abs(px) > 2 && abs(py) > 2 && glm::dot(glm::normalize(test), fwd) < 0.1f)
				continue;

			component->RenderComponent(i, j, cam);
		}
	}

	component->PostRenderComponent();
}

void Engine::Terrain::RenderTiledComponentShadow(TerrainComponent* component, Camera* cam, const glm::mat4& proj)
{
	glm::vec3 cameraPosition = cam->getPosition();

	int x = -int((floor(cameraPosition.x)) / tileWidth);
	int y = -int((floor(cameraPosition.z)) / tileWidth);

	unsigned int rr = component->GetRenderRadius();
	int xStart = x - rr;
	int xEnd = x + rr;
	int yStart = y - rr;
	int yEnd = y + rr;

	component->PreRenderComponent();

	Program* prog = component->GetShadowMapProgram();
	prog->Use();
	prog->ApplyGlobalUniforms();

	// Culling parameters
	glm::vec3 fwd = cam->getForwardVector();
	fwd.y = 0;
	fwd = -glm::normalize(fwd);
	int px = -int(renderRadius), py = px;

	for (int i = xStart; i < xEnd; i++, px++)
	{
		for (int j = yStart; j < yEnd; j++, py++)
		{
			// Culling (skips almost half)
			glm::vec3 test(i - x, 0, j - y);
			if (abs(px) > 2 && abs(py) > 2 && glm::dot(glm::normalize(test), fwd) < 0.1f)
				continue;

			component->RenderShadow(proj, i, j , cam);
		}
	}

	component->PostRenderComponent();

}