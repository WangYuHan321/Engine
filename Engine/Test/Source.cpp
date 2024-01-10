#include "../Util/common.h"
#include "../Table/ShaderTable.h"
#include "../Table/MeshTable.h"
#include "../Table/ProgramTable.h"
#include "../Window/Context/Device.h"
#include "../Window/WindowManager.h"
#include "../Window/Input/InputManager.h"
#include "../Render/Terrain/Terrain.h"
#include "../Render/Camera/Camera.h"
#include "../Scene/Scene.h"
#include "../Render/DeferredRender/DeferredRenderer.h"
#include "../Render/Renderer.h"
#include "../Config/WorldConfig.h"
#include "../Render/Program/DebugPorgram.h"
#include "../Render/Program/ProceduralTerrainProgram.h"
#include "../Render/Shadow/CascadeShadowMaps.h"
#include "../Render/Init/NoiseInitializer.h"
#include "../Util/Time.h"
#include "../Render/Notifier/RenderableNotifier.h"
#include "../Render/Cloud/VolumetricClouds.h"
#include "../Render/Program/SkyProgram.h"
#include "../Render/Terrain/Component/WaterComponent.h"
#include "../Render/Program/PostProcessProgram/SSReflectionProgram.h"
#include "../Render/Program/PostProcessProgram/SSGodRayProgram.h"
#include "../Render/Program/PostProcessProgram/CloudFilterProgram.h"
#include "../Render/Program/PostProcessProgram/DeferredShadingProgram.h"

void initTable();
void initRender();
void initScene();
void initNoise();
void initWindow(Engine::uint32 width, Engine::uint32 height);

int main()
{
	initWindow(900, 600);
	initTable();
	initScene();
	initRender();
	initNoise();

	std::pair<double, double> oldpos = Engine::Window::Input::InputManager::getInstance()->GetMousePosition();
	glfwSetCursor(Engine::Window::WindowManager::getInstance()->GetWindow(), NULL);
	glfwSetInputMode(Engine::Window::WindowManager::getInstance()->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	while (!Engine::Window::WindowManager::getInstance()->ShouldClose())
	{

		Engine::Device::getInstance()->PollEvents();
		if(Engine::Window::Input::InputManager::getInstance()->IsKeyReleased(Engine::Window::Input::EKey::KEY_ESCAPE))
		{
			Engine::Window::WindowManager::getInstance()->SetShouldClose(true);
			break;
		}

		if (Engine::Window::Input::InputManager::getInstance()->IsKeyReleased(Engine::Window::Input::EKey::KEY_F1))
		{
			++Engine::Setting::debugIndex;
			if (Engine::Setting::debugIndex == 6)
				Engine::Setting::debugIndex = 0;
			
		}

		if (Engine::Window::Input::InputManager::getInstance()->IsKeyPressed(Engine::Window::Input::EKey::KEY_W))
		{
			Engine::SceneManager::getInstance()->GetActiveScene()->GetCamera()->translateView(glm::vec3(0.0f, 0.0f, 0.05f));
		}
		if (Engine::Window::Input::InputManager::getInstance()->IsKeyPressed(Engine::Window::Input::EKey::KEY_S))
		{
			Engine::SceneManager::getInstance()->GetActiveScene()->GetCamera()->translateView(glm::vec3(0.0f, 0.0f, -0.05f));
		}
		if (Engine::Window::Input::InputManager::getInstance()->IsKeyPressed(Engine::Window::Input::EKey::KEY_A))
		{
			Engine::SceneManager::getInstance()->GetActiveScene()->GetCamera()->translateView(glm::vec3(0.05f, 0.0f, 0.0f));
		}
		if (Engine::Window::Input::InputManager::getInstance()->IsKeyPressed(Engine::Window::Input::EKey::KEY_D))
		{
			Engine::SceneManager::getInstance()->GetActiveScene()->GetCamera()->translateView(glm::vec3(-0.05f, 0.0f, 0.05f));
		}

		if (Engine::Window::Input::InputManager::getInstance()->IsKeyPressed(Engine::Window::Input::EKey::KEY_E))
		{
			Engine::SceneManager::getInstance()->GetActiveScene()->GetCamera()->translateView(glm::vec3(0.0f, 0.05f, 0.0f));
		}
		if (Engine::Window::Input::InputManager::getInstance()->IsKeyPressed(Engine::Window::Input::EKey::KEY_F))
		{
			Engine::SceneManager::getInstance()->GetActiveScene()->GetCamera()->translateView(glm::vec3(0.0f, -0.05f, 0.05f));
		}

		if (Engine::Window::Input::InputManager::getInstance()->IsKeyPressed(Engine::Window::Input::EKey::KEY_F9))
		{
			Engine::Setting::drawingMethod += 1;
			if (Engine::Setting::drawingMethod >= 3)
				Engine::Setting::drawingMethod = 0;
			Engine::RenderableNotifier::getInstance()->CheckUpdateConfig();
		}

		if (Engine::Window::Input::InputManager::getInstance()->GetMousePosition()
			!= oldpos)
		{
			int deltaX = oldpos.first - Engine::Window::Input::InputManager::getInstance()->GetMousePosition().first;
			int deltaY = oldpos.second - Engine::Window::Input::InputManager::getInstance()->GetMousePosition().second;

			oldpos.first = Engine::Window::Input::InputManager::getInstance()->GetMousePosition().first;
			oldpos.second = Engine::Window::Input::InputManager::getInstance()->GetMousePosition().second;

			float hAngle = -deltaX * 0.01f;
			float vAngle = -deltaY * 0.01f;

			float speed = 10.0f * Engine::Time::deltaTime;
			Engine::SceneManager::getInstance()->GetActiveScene()->GetCamera()->rotateView(glm::vec3(vAngle * speed, hAngle * speed, 0.0f));
		}


		Engine::RenderManager::getInstance()->DoRender();

		Engine::Window::WindowManager::getInstance()->SwapBuffer();
		Engine::Time::update(glfwGetTime());
	}


	return 0;
}

void initNoise()
{
	Engine::CloudSystem::NoiseInitializer::getInstance()->Init();
	Engine::CloudSystem::NoiseInitializer::getInstance()->Render();
}

Engine::PostProcessChainNode* createSSGodRayNode()
{
	Engine::PostProcessChainNode* node = new Engine::PostProcessChainNode;

	// Shader
	node->postProcessProgram = Engine::ProgramTable::getInstance()->GetProgram<Engine::SSGodRayProgram>();

	// RTT
	node->renderBuffer = new Engine::DeferredRenderObject(1, false);
	node->renderBuffer->AddColorBuffer(0, GL_RGBA16F, GL_RGBA, GL_FLOAT, 500, 500, "", GL_LINEAR);
	node->renderBuffer->AddDepthBuffer24(500, 500);
	node->callBack = 0;

	// Render plane
	Engine::Mesh* mi = Engine::MeshTable::getInstance()->GetMesh("plane");
	if (mi != 0)
	{
		node->postProcessProgram->ConfigureMeshBuffer(mi);
		node->obj = new Engine::PostProcessObject(mi);
	}

	return node;
}

Engine::PostProcessChainNode* createSSReflectionNode()
{
	Engine::PostProcessChainNode* node = new Engine::PostProcessChainNode;

	// Shader
	node->postProcessProgram = Engine::ProgramTable::getInstance()->GetProgram<Engine::SSReflectionProgram>();

	// RTT
	node->renderBuffer = new Engine::DeferredRenderObject(1, false);
	node->renderBuffer->AddColorBuffer(0, GL_RGBA16F, GL_RGBA, GL_FLOAT, 500, 500, "", GL_LINEAR);
	node->renderBuffer->AddDepthBuffer24(500, 500);
	node->callBack = 0;

	// Render plane
	Engine::Mesh* mi = Engine::MeshTable::getInstance()->GetMesh("plane");
	if (mi != 0)
	{
		node->postProcessProgram->ConfigureMeshBuffer(mi);
		node->obj = new Engine::PostProcessObject(mi);
	}

	return node;
}

void initRender()
{
	Engine::DeferredRenderer* df = new Engine::DeferredRenderer();

	Engine::ScreenManager::SCREEN_WIDTH = Engine::ScreenManager::REAL_SCREEN_WIDTH = 900;
	Engine::ScreenManager::SCREEN_HEIGHT = Engine::ScreenManager::REAL_SCREEN_HEIGHT = 600;

	df->AddPostProcess(createSSGodRayNode());
	df->AddPostProcess(createSSReflectionNode());

	Engine::RenderManager::getInstance()->SetRenderer(df);
	Engine::Window::WindowManager::getInstance()->SetWindowSize(glm::vec2(900, 600));
	Engine::RenderManager::getInstance()->DoReSize(900, 600);

}

void initScene()
{
	Engine::Scene* sc = new Engine::Scene();
	Engine::Camera* cam = new Engine::Camera(0.5f, 1000.0f, 35.0f);
	cam->translateView(glm::vec3(0.0f, -7.0f, 0.0f));
	sc->SetCamera(cam);
	
	Engine::DirectionalLight* dl = new Engine::DirectionalLight();
	dl->SetColor(Engine::Setting::lightColor);
	dl->SetDirection(Engine::Setting::lightDirection);
	sc->SetDirectionLight(dl);

	Engine::SceneManager::getInstance()->RegisterScene("scene_0", sc);
	Engine::SceneManager::getInstance()->SetActiveScene(sc);
	sc->Initialize();

	Engine::CascadeShadowMaps::getInstance()->Init();
}

void initTable()
{
	glewInit();

	std::shared_ptr<Engine::StorageTable> t =  Engine::ShaderTable::getInstance();
	std::shared_ptr<Engine::StorageTable> tt = Engine::MeshTable::getInstance();
	std::shared_ptr<Engine::StorageTable> ttt = Engine::ProgramTable::getInstance();
	
	Engine::MeshTable::getInstance()->Initialize();

	Engine::ProgramTable::getInstance()->RegisterProgramFactory(Engine::ProceduralTerrainProgram::PROGRAM_NAME, new Engine::ProceduralTerrainProgramFactory());
	Engine::ProgramTable::getInstance()->RegisterProgramFactory(Engine::ProceduralWaterProgram::PROGRAM_NAME, new Engine::ProceduralWaterProgramFactory());
	Engine::ProgramTable::getInstance()->RegisterProgramFactory(Engine::DeferredShadingProgram::PROGRAM_NAME, new Engine::DeferredShadingProgramFactory());
	Engine::ProgramTable::getInstance()->RegisterProgramFactory(Engine::DebugPorgram::PROGRAM_NAME, new Engine::DebugProgramFactory());
	Engine::ProgramTable::getInstance()->RegisterProgramFactory(Engine::SSGodRayProgram::PROGRAM_NAME, new Engine::SSGodRayProgramFactory());
	Engine::ProgramTable::getInstance()->RegisterProgramFactory(Engine::SSReflectionProgram::PROGRAM_NAME, new Engine::SSReflectionProgramFactory());
	Engine::ProgramTable::getInstance()->RegisterProgramFactory(Engine::PostProcessProgram::PROGRAM_NAME, new Engine::PostProcessProgramFactory());
	Engine::ProgramTable::getInstance()->RegisterProgramFactory(Engine::VolumetricCloudProgram::PROGRAM_NAME, new Engine::VolumetricCloudProgramFactory());
	Engine::ProgramTable::getInstance()->RegisterProgramFactory(Engine::SkyProgram::PROGRAM_NAME, new Engine::SkyProgramFactory());
	Engine::ProgramTable::getInstance()->RegisterProgramFactory(Engine::CloudFilterProgram::PROGRAM_NAME, new Engine::CloudFilterProgramFactory());

	Engine::TableManger::getInstance()->RegisterTable(dynamic_cast<Engine::StorageTable*>(t.get()));
	Engine::TableManger::getInstance()->RegisterTable(dynamic_cast<Engine::StorageTable*>(tt.get()));
	Engine::TableManger::getInstance()->RegisterTable(dynamic_cast<Engine::StorageTable*>(ttt.get()));
}

void initWindow(Engine::uint32 width, Engine::uint32 height)
{
	Engine::Device::getInstance()->OnInit();
	Engine::Window::Input::InputManager::getInstance()->OnStartUp();

	Engine::Window::WindowManager::getInstance()->CreateGLFWWindow(width, height);
	Engine::Window::WindowManager::getInstance()->BindKeyCallback();
	Engine::Window::WindowManager::getInstance()->BindMouseCallback();
	Engine::Window::WindowManager::getInstance()->BindCloseCallback();
	Engine::Window::WindowManager::getInstance()->BindResizeCallback();
	Engine::Window::WindowManager::getInstance()->BindMousePosCallback();
	Engine::Window::WindowManager::getInstance()->BindFrameBufferCallback();
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
	fprintf(stdout, "Status: Using GLFW %s\n", glfwGetVersionString());

	Engine::Window::WindowManager::getInstance()->ReSizeWindowEvent.AddListenerID(std::bind(&Engine::Window::WindowManager::OnResize,
		Engine::Window::WindowManager::getInstance(), std::placeholders::_1, std::placeholders::_2));

}






