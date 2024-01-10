#pragma once

#include "../Renderer.h"
#include "../DeferredRender/DeferredRenderer.h"

namespace Engine
{
	class SideBySideRenderer :
		public Renderer
	{
	private:

		DeferredRenderer* leftRenderer;
		DeferredRenderer* rightRenderer;

		bool initialized;
		bool clearScreen;
		int c;

	public:
		SideBySideRenderer();
		~SideBySideRenderer();

		void AddLeftPostProcess();
		void AddRightPostProcess();

		void Initialize();
		void DoRender();

		void OnReSize(uint32 w, uint32 h);
	};
}


