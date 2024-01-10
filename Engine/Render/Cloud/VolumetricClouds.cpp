#include "VolumetricClouds.h"

#include "../../Table/MeshTable.h"
#include "../../Table/ProgramTable.h"
#include "../../Util/Time.h"

Engine::CloudSystem::VolumetricClouds::VolumetricClouds()
{
	volumeCloudProgram = Engine::ProgramTable::getInstance()->GetProgram<Engine::VolumetricCloudProgram>();
	filterProgram = Engine::ProgramTable::getInstance()->GetProgram<Engine::CloudFilterProgram>();
	renderPlane = Engine::MeshTable::getInstance()->GetMesh("plane");

	volumeCloudProgram->ConfigureMeshBuffer(renderPlane);
	filterProgram->ConfigureMeshBuffer(renderPlane);

	for (int i = 0; i < 2; i++)
	{
		reprojectionBuffer[i] = new Engine::DeferredRenderObject(2, false);
		reprojectionBuffer[i]->SetReSizeMod(0.5f);
		reproBuffer[i] = reprojectionBuffer[i]->AddColorBuffer(0, GL_RGBA16F, GL_RGBA, GL_FLOAT, 512, 1024, "", GL_NEAREST);
		//reproBuffer[i]->setSComponentWrapType(GL_CLAMP_TO_EDGE);
		//reproBuffer[i]->setTComponentWrapType(GL_CLAMP_TO_EDGE);
		reprojectionBuffer[i]->AddColorBuffer(1, GL_RGB32F, GL_RGBA, GL_UNSIGNED_BYTE, 512, 1024, "", GL_NEAREST);
		reprojectionBuffer[i]->AddDepthBuffer24(512, 1024);
		reprojectionBuffer[i]->Initialize();
	}
}

void Engine::CloudSystem::VolumetricClouds::Render(Camera* cam)
{
	int prevFBO;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFBO);
	glDisable(GL_DEPTH_TEST);

	renderPlane->use();

	int frameMod = Engine::Time::frame % 2;

	// Render clouds
	glBindFramebuffer(GL_FRAMEBUFFER, reprojectionBuffer[frameMod]->GetFrameBufferID());

	glClear(GL_COLOR_BUFFER_BIT);
	volumeCloudProgram->Use();
	volumeCloudProgram->OnRenderObject(NULL, cam);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	//Filter clouds
	glBindFramebuffer(GL_FRAMEBUFFER, prevFBO);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	filterProgram->Use();
	filterProgram->SetBufferInput(&reproBuffer[0]);
	filterProgram->OnRenderObject(NULL, cam);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void Engine::CloudSystem::VolumetricClouds::RenderShadow(Camera* camera, const glm::mat4& projectionMatrix)
{

}