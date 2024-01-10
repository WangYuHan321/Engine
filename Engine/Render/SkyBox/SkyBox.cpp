#include "SkyBox.h"
#include "../../Config/WorldConfig.h"
#include "../Cloud/VolumetricClouds.h"
#include "../../Table/ProgramTable.h"
#include "../../Table/MeshTable.h"

Engine::SkyBox::SkyBox()
{
	Initialize();
	clouds = new Engine::CloudSystem::VolumetricClouds();
	renderMode = GL_TRIANGLES;
}

Engine::SkyBox::~SkyBox()
{
	if (cubeMesh != NULL)
	{
		delete cubeMesh;
	}
}

void Engine::SkyBox::Render(Camera* cam)
{
	glDepthFunc(GL_LEQUAL);

	const Engine::Mesh* data = cubeMesh->GetMesh();
	data->use();

	skyProgram->Use();

	const glm::vec3 pos = cam->getPosition();
	glm::vec3 cubePos(-pos);
	cubeMesh->SetTranslation(cubePos);
	skyProgram->OnRenderObject(cubeMesh, cam);

	glDrawElements(renderMode, data->getNumFaces() * data->getNumVerticesPerFace(), GL_UNSIGNED_INT, (void*)0);

	glDepthFunc(GL_LESS);

	if (Engine::Setting::drawClouds == 0)
	{
		// Render clouds
		clouds->Render(cam);
	}
}

void Engine::SkyBox::NotifyRenderModeUpdate(RenderMode mode)
{
	switch (mode)
	{
	case Engine::RenderMode::RENDER_MODE_WIRE:
		renderMode = GL_LINE;
		break;
	default:
		renderMode = GL_TRIANGLES;
		break;
	}
}

void Engine::SkyBox::Initialize()
{
	skyProgram = Engine::ProgramTable::getInstance()->GetProgram<Engine::SkyProgram>();
	Engine::Mesh* mesh = Engine::MeshTable::getInstance()->GetMesh("cube");

	skyProgram->ConfigureMeshBuffer(mesh);

	cubeMesh = new Engine::Object(mesh);
}