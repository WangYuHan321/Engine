#include "LandscapeComponent.h"
#include "../../Render/Shadow/CascadeShadowMaps.h"
#include "../../../Table/ProgramTable.h"
#include "../../../Table/MeshTable.h"

Engine::LandscapeComponent::LandscapeComponent():
	Engine::TerrainComponent()
{
}

void Engine::LandscapeComponent::Initialize()
{
	fillProgram = ProgramTable::getInstance()->GetProgram<ProceduralTerrainProgram>();
	wireProgram = ProgramTable::getInstance()->GetProgram<ProceduralTerrainProgram>
		(ProceduralTerrainProgram::WIRE_DRAW_MODE);
	pointProgram = ProgramTable::getInstance()->GetProgram<ProceduralTerrainProgram>
		(ProceduralTerrainProgram::POINT_DRAW_MODE);
	shadowProgram = ProgramTable::getInstance()->GetProgram<ProceduralTerrainProgram>
		(ProceduralTerrainProgram::SHADOW_MAP);

	Engine::Mesh* tile = Engine::MeshTable::getInstance()->GetMesh("terrain_tile");
	landscapeTile = new Engine::Object(tile);
	landscapeTile->SetScale(glm::vec3(scale));

	fillProgram->ConfigureMeshBuffer(tile);
	wireProgram->ConfigureMeshBuffer(tile);
	pointProgram->ConfigureMeshBuffer(tile);
	shadowProgram->ConfigureMeshBuffer(tile);

	activeProgram = fillProgram;
}

Engine::uint32 Engine::LandscapeComponent::GetRenderRadius()
{
	return 12;
}

void Engine::LandscapeComponent::PreRenderComponent()
{
	glBindVertexArray(landscapeTile->GetMesh()->vao);
}

void Engine::LandscapeComponent::RenderComponent(int i, int j, Engine::Camera* camera)
{
	float poxX = i * scale;
	float posZ = j * scale;
	landscapeTile->SetTranslation(glm::vec3(poxX, 0.0f, posZ));

	activeProgram->SetUniformGridPosition(i, j);
	activeProgram->SetUniformLightDepthMatrix(Engine::CascadeShadowMaps::getInstance()->GetDepthMatrix0() * landscapeTile->GetModelMatrix());
	activeProgram->SetUniformLightDepthMatrix1(Engine::CascadeShadowMaps::getInstance()->GetDepthMatrix1() * landscapeTile->GetModelMatrix());

	activeProgram->OnRenderObject(landscapeTile, camera);

	glDrawElements(GL_PATCHES, 6, GL_UNSIGNED_INT, (void*)0);
}

void Engine::LandscapeComponent::RenderShadow(const glm::mat4& projection, int i, int j, Engine::Camera* cam)
{
	float poxX = i * scale;
	float posZ = j * scale;
	landscapeTile->SetTranslation(glm::vec3(poxX, 0.0f, posZ));

	shadowProgram->SetUniformGridPosition(i, j);
	shadowProgram->SetUniformLightDepthMatrix(projection * landscapeTile->GetModelMatrix());

	shadowProgram->OnRenderObject(landscapeTile, cam);

	glDrawElements(GL_PATCHES, 6, GL_UNSIGNED_INT, (void*)0);
}

void Engine::LandscapeComponent::NotifyRenderModeChange(Engine::RenderMode mode)
{
	switch (mode)
	{
	case Engine::RenderMode::RENDER_MODE_SHADED:
		activeProgram = fillProgram;
		break;
	case Engine::RenderMode::RENDER_MODE_WIRE:
		activeProgram = wireProgram;
		break;
	case Engine::RenderMode::RENDER_MODE_POINT:
		activeProgram = pointProgram;
		break;
	}
}