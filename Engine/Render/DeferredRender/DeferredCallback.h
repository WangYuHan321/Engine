#pragma once

#include "../Object/Object.h"
#include "../Program/Program.h"
#include "DeferredRenderObject.h"

namespace Engine
{
	class DeferredCallback
	{
		public:
			DeferredCallback();
			~DeferredCallback();

			virtual void Initialize(Object* obj, Program* program, DeferredRenderObject* buffer);
			virtual void Execute(Object* obj, Program* program, DeferredRenderObject* buffer, Camera* cam) = 0;
	};
}


