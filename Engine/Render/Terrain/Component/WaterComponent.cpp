#include "WaterComponent.h"
#include "../../Config/WorldConfig.h"
#include "../../Render/Shadow/CascadeShadowMaps.h"
#include "../../../Table/ProgramTable.h"
#include "../../../Table/MeshTable.h"

Engine::WaterComponent::WaterComponent():
	Engine::TerrainComponent()
{
}

void Engine::WaterComponent::Initialize()
{
	fillProgram = Engine::ProgramTable::getInstance()->GetProgram<ProceduralWaterProgram>();

	wireProgram = Engine::ProgramTable::getInstance()->GetProgram<ProceduralWaterProgram>(Engine::ProceduralWaterProgram::WIRE_DRAW_MODE);

	pointProgram = Engine::ProgramTable::getInstance()->GetProgram<ProceduralWaterProgram>(Engine::ProceduralWaterProgram::POINT_DRAW_MODE);

	//shadowProgram = Engine::ProgramTable::getInstance()->GetProgram<ProceduralWaterProgram>(Engine::ProceduralWaterProgram::SHADOW_MAP);

	Engine::Mesh* tile = Engine::MeshTable::getInstance()->GetMesh("terrain_tile");

	waterTile = new Engine::Object(tile);
	waterTile->SetScale(glm::vec3(scale));

	fillProgram->ConfigureMeshBuffer(tile);
	wireProgram->ConfigureMeshBuffer(tile);
	pointProgram->ConfigureMeshBuffer(tile);
	//shadowProgram->ConfigureMeshBuffer(tile);
	
	activeWaterProgram = fillProgram;
}

Engine::uint32 Engine::WaterComponent::GetRenderRadius()
{
	return 12;
}

Engine::Program* Engine::WaterComponent::GetActiveProgram()
{
	return activeWaterProgram;
}

Engine::Program* Engine::WaterComponent::GetShadowMapProgram()
{
	return shadowProgram;
}

void Engine::WaterComponent::PreRenderComponent()
{
	glBindVertexArray(waterTile->GetMesh()->vao);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Engine::WaterComponent::RenderComponent(int i, int j, Engine::Camera* camera)
{
	float poxX = i * scale;
	float posZ = j * scale;
	waterTile->SetTranslation(glm::vec3(poxX, Engine::Setting::waterHeight * scale * 1.5f, posZ));

	activeWaterProgram->SetUniformGridPosition(i, j);
	activeWaterProgram->SetUniformLightDepthMatrix(Engine::CascadeShadowMaps::getInstance()->GetDepthMatrix0() * waterTile->GetModelMatrix());
	activeWaterProgram->SetUniformLightDepthMatrix1(Engine::CascadeShadowMaps::getInstance()->GetDepthMatrix1() * waterTile->GetModelMatrix());
	activeWaterProgram->OnRenderObject(waterTile, camera);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
}

void Engine::WaterComponent::RenderShadow(const glm::mat4& projection, int i, int j, Engine::Camera* cam)
{
	//float poxX = i * scale;
	//float posZ = j * scale;
	//waterTile->SetTranslation(glm::vec3(poxX, Engine::Setting::waterHeight * scale * 1.5f, posZ));

	//shadowProgram->SetUniformGridPosition(i, j);
	//shadowProgram->SetUniformLightDepthMatrix(Engine::CascadeShadowMaps::getInstance()->GetDepthMatrix0() * waterTile->GetModelMatrix());
	//shadowProgram->SetUniformLightDepthMatrix1(Engine::CascadeShadowMaps::getInstance()->GetDepthMatrix1() * waterTile->GetModelMatrix());
	//shadowProgram->OnRenderObject(waterTile, cam);

	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
}

void Engine::WaterComponent::PostRenderComponent()
{
	glDisable(GL_BLEND);
}

void Engine::WaterComponent::NotifyRenderModeChange(Engine::RenderMode mode)
{
	switch (mode)
	{
	case Engine::RenderMode::RENDER_MODE_SHADED:
		activeWaterProgram = fillProgram;
		break;
	case Engine::RenderMode::RENDER_MODE_WIRE:
		activeWaterProgram = wireProgram;
		break;
	case Engine::RenderMode::RENDER_MODE_POINT:
		activeWaterProgram = pointProgram;
		break;
	}
}