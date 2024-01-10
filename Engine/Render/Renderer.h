#pragma once

#include "../Util/common.h"
#include "Camera/Camera.h"
#include "../Util/Singleton.h"

namespace Engine
{
	class ScreenManager
	{
	public:
		static unsigned int SCREEN_WIDTH;
		static unsigned int SCREEN_HEIGHT;

		static unsigned int REAL_SCREEN_WIDTH;
		static unsigned int REAL_SCREEN_HEIGHT;
	};

	class Renderer
	{
	protected:
		Camera* activeCam;

	public:

		Renderer();
		~Renderer();

		void RenderFromCamera(Camera* cam);

		virtual void Initialize();
		virtual void DoRender() = 0;

		virtual void OnReSize(uint32 w, uint32 h) = 0;
	};


	class RenderManager : public CSingleton<RenderManager>
	{
	private:
		Renderer* activeRender;

	public:

		RenderManager();
		~RenderManager();

		void SetRenderer(Renderer* render);
		Renderer* GetRenderer();
		void DoRender();
		void DoReSize(uint32 w, uint32 h);

	};
}


