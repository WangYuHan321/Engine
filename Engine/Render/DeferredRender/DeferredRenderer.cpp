#include "DeferredRenderer.h"
#include "../../Scene/Scene.h"
#include "../../Table/ProgramTable.h"
#include "../../Table/MeshTable.h"
#include "../Shadow/CascadeShadowMaps.h"
#include "../../Table/DeferredRenderObjectsTable.h"
#include "../../Render/Program/DebugPorgram.h"
#include "../Program/PostProcessProgram/DeferredShadingProgram.h"

Engine::DeferredRenderer::DeferredRenderer():
	Engine::Renderer()
{
	initialized = false;

	renderFunc = &DeferredRenderer::InitializeLoop;
}

Engine::DeferredRenderer::~DeferredRenderer()
{

}

void Engine::DeferredRenderer::AddPostProcess(PostProcessChainNode* object)
{
	postProcessChain.push_back(object);
}

void Engine::DeferredRenderer::RunPostProcess()
{
	glDisable(GL_DEPTH_TEST);

	std::list<Engine::PostProcessChainNode*>::iterator it = postProcessChain.begin();

	while (it != postProcessChain.end())
	{
		Engine::PostProcessChainNode* node = *it;

		Engine::DeferredRenderObject* buffer = node->renderBuffer;
		glBindFramebuffer(GL_FRAMEBUFFER, buffer->GetFrameBufferID());

		Engine::Program* prog = node->postProcessProgram;
		prog->Use();

		node->obj->GetMesh()->use();

		prog->OnRenderObject(node->obj, activeCam);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		it++;

	}

	glEnable(GL_DEPTH_TEST);
}

void Engine::DeferredRenderer::Initialize()
{
	Engine::Renderer::Initialize();
	
	if (initialized)
		return;

	initialized = true;

	//创建GBUFFER
	forwardPassBuffer = new Engine::DeferredRenderObject(6, true);
	//GBUFFER COLOR => GL_RGBA16F - GL_FLOAT
	gBufferColor = forwardPassBuffer->AddColorBuffer(0, GL_RGBA16F, GL_RGBA, GL_FLOAT, 500, 500, Engine::DeferredRenderObject::G_BUFFER_COLOR, GL_NEAREST);
	//GPU使用格式 GL_RGB32F 原始数据 GL_RGBA， 单个像素pixel type GL_UNSIGNED_BYTE
	gBufferNormal = forwardPassBuffer->AddColorBuffer(1, GL_RGB32F, GL_RGBA, GL_UNSIGNED_BYTE, 500, 500, Engine::DeferredRenderObject::G_BUFFER_NORMAL, GL_NEAREST);
	//GPU使用格式 GL_RGBA8 原始数据 GL_RGBA 单个像素 GL_FLOAT
	gBufferSpecular = forwardPassBuffer->AddColorBuffer(2, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, Engine::DeferredRenderObject::G_BUFFER_SPECULAR, GL_NEAREST);
	//GPU使用格式 GL_RGBA16F 原始数据 GL_RGBA 单个像素 GL_FLOAT
	gBufferEmissive = forwardPassBuffer->AddColorBuffer(3, GL_RGBA16F, GL_RGBA, GL_FLOAT, 500, 500, Engine::DeferredRenderObject::G_BUFFER_EMISSIVE, GL_NEAREST);
	//GPU使用格式 GL_RGB32F 原始数据 GL_RGBA 单个像素 GL_UNSIGNED_BYTE
	gBufferPos = forwardPassBuffer->AddColorBuffer(4, GL_RGB32F, GL_RGBA, GL_UNSIGNED_BYTE, 500, 500, Engine::DeferredRenderObject::G_BUFFER_POS, GL_NEAREST);
	//GPU使用格式 GL_RGBA8 原始数据 GL_RGBA 单个像素 GL_FLOAT
	gBufferInfo = forwardPassBuffer->AddColorBuffer(5, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, "InfoBuffer", GL_LINEAR);
	gBufferDepth = forwardPassBuffer->AddDepthBuffer24(500, 500);
	forwardPassBuffer->Initialize();

	//创建延迟渲染buffer
	deferredPassBuffer = new Engine::DeferredRenderObject(3, false);
	deferredPassBuffer->AddColorBuffer(0, GL_RGBA16F, GL_RGBA, GL_FLOAT, 500, 500, "", GL_NEAREST);
	deferredPassBuffer->AddColorBuffer(1, GL_RGBA16F, GL_RGBA, GL_FLOAT, 500, 500, "", GL_NEAREST);
	deferredPassBuffer->AddColorBuffer(2, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, "", GL_NEAREST);
	deferredPassBuffer->AddDepthBuffer24(500, 500);
	deferredPassBuffer->Initialize();

	//创建延迟渲染程序
	deferredShading = Engine::ProgramTable::getInstance()->GetProgram<Engine::DeferredShadingProgram>();
	Engine::Mesh* mi = Engine::MeshTable::getInstance()->GetMesh("plane");
	deferredShading->ConfigureMeshBuffer(mi);

	// Link 到GBUFFER 上
	deferredDrawSurface = new Engine::PostProcessObject(mi);
	forwardPassBuffer->PopulateDeferredObject(deferredDrawSurface);

	//屏幕PostProcess
	screenOutput = Engine::ProgramTable::getInstance()->GetProgram<Engine::PostProcessProgram>();
	screenOutput->ConfigureMeshBuffer(mi);

	//后加 debug
	debugObj = new Engine::PostProcessObject(mi);
	deferredPassBuffer->PopulateDeferredObject(debugObj);

	debugScreenOutput = Engine::ProgramTable::getInstance()->GetProgram<Engine::DebugPorgram>();
	debugScreenOutput->ConfigureMeshBuffer(mi);
	//后加 debug


	//链接后处理作为一个chain
	std::list<Engine::PostProcessChainNode*>::iterator it = postProcessChain.begin();
	Engine::DeferredRenderObject* previousLink = deferredPassBuffer;
	while (it != postProcessChain.end())
	{
		Engine::PostProcessChainNode* node = *it;
		node->renderBuffer->Initialize();
		previousLink->PopulateDeferredObject(node->obj);
		previousLink = node->renderBuffer;


		if (node->callBack != 0)
		{
			node->callBack->Initialize(node->obj, node->postProcessProgram, node->renderBuffer);
		}

		it++;
	}

	chainEnd = new Engine::PostProcessObject(mi);
	previousLink->PopulateDeferredObject(chainEnd);

}

void Engine::DeferredRenderer::DoRender()
{
	(this->*renderFunc)();
}

void Engine::DeferredRenderer::OnReSize(uint32 w, uint32 h)
{
	Engine::DeferredRenderObjectsTable::getInstance()->OnReSize(w, h);
}

void Engine::DeferredRenderer::InitializeLoop()
{
	CascadeShadowMaps::getInstance()->InitializeFrame(activeCam);

	Scene* scenePtr = Engine::SceneManager::getInstance()->GetActiveScene();

	//执行前向渲染
	glBindFramebuffer(GL_FRAMEBUFFER, forwardPassBuffer->GetFrameBufferID());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	scenePtr->GetTerrain()->Render(activeCam);

	//执行延迟渲染 pass
	glDisable(GL_CULL_FACE);
	glBindFramebuffer(GL_FRAMEBUFFER, deferredPassBuffer->GetFrameBufferID());
	glClear(GL_DEPTH_BUFFER_BIT);
	deferredShading->Use();
	deferredDrawSurface->GetMesh()->use();
	deferredShading->OnRenderObject(deferredDrawSurface, activeCam);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	scenePtr->GetSkyBox()->Render(activeCam);

	RunPostProcess();

	// Enable default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_DEPTH_BUFFER_BIT);

	// Output the final result to screen
	screenOutput->Use();
	chainEnd->GetMesh()->use();
	screenOutput->OnRenderObject(chainEnd, activeCam);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

}
