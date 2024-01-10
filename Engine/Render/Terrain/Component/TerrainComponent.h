#pragma once

#include "../../Program/Program.h"

namespace Engine
{
	class TerrainComponent
	{
	protected:
		float scale;
		bool isShowable;

	public:
		TerrainComponent()
		{
		}

		void Init(float scale, bool shadowable)
		{
			this->scale = scale;
			this->isShowable = shadowable;
			Initialize();
		}

		bool CastShadows()
		{
			return isShowable;
		}

		virtual void Initialize(){}

		virtual uint32 GetRenderRadius() = 0;

		virtual Program* GetActiveProgram() { return  nullptr; }

		virtual Program* GetShadowMapProgram() { return  nullptr; }

		virtual void PreRenderComponent()
		{
	
		}

		virtual void RenderComponent(int i, int j, Engine::Camera* camera)
		{

		}

		virtual void PostRenderComponent()
		{

		}

		virtual void RenderShadow(const glm::mat4& projection, int i, int j, Engine::Camera* cam)
		{

		}

		virtual void NotifyRenderModeChange(Engine::RenderMode mode)
		{

		}
	};
}


