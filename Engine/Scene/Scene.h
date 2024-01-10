#pragma once

#include "../Render/Camera/Camera.h"
#include "../Util/Singleton.h"
#include "../Util/common.h"
#include "../Render/Terrain/Terrain.h"
#include "../Render/SkyBox/SkyBox.h"
#include "../Render/Light/DirectionalLight.h"
#include "../Render/Light/PointLight.h"
#include "../Render/Light/SpotLight.h"

namespace Engine
{
	class ProgramRenderables
	{

	};

	class Scene
	{
	private:
		Camera* camera;
		Terrain* terrain;
		DirectionalLight *directionLight;
		AbstractSkyBox* skyBox;
		std::map<std::string, PointLight*> pointLights;
		std::map<std::string, SpotLight*> spotLights;

	public:

		Scene();
		~Scene();

		void Initialize();

		inline Camera* GetCamera() { return camera; }
		inline void SetCamera(Camera* cam) { camera = cam; }
		inline DirectionalLight* GetDirectionLight() { return directionLight; }
		inline void SetDirectionLight(DirectionalLight* dl) { directionLight = dl; }
		inline std::map<std::string, PointLight*>& GetPointLights() { return pointLights; }
		inline void SetPointLight(std::map<std::string, PointLight*> pl) { pointLights = pl; }
		inline std::map<std::string, SpotLight*>& GetSpotLights() { return spotLights; }
		inline void SetSpotLight(std::map<std::string, SpotLight*> sl) { spotLights = sl; }
		inline Terrain* GetTerrain() { return terrain; }
		inline AbstractSkyBox* GetSkyBox() { return skyBox; }

	};

	class SceneManager : public CSingleton<SceneManager>
	{
	private:
		std::map<string, Scene*> scenes;

		Scene* activeScene;

	public:

		SceneManager();
		~SceneManager();

		void RegisterScene(string sceneName, Scene* scene);
		inline void SetActiveScene(Scene* scene) { activeScene = scene; }

		inline Scene* GetActiveScene() { return activeScene; }


	};

}


