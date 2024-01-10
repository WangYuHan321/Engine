#pragma once

#include "StorageTable.h"
#include"../Render/DeferredRender/DeferredRenderObject.h"

namespace Engine
{
	class DeferredRenderObjectsTable : public StorageTable,
		public CSingleton<DeferredRenderObjectsTable>
	{
	private:
		std::list<DeferredRenderObject*> deferredRenderObjects;

	public:
		void RegisterDeferredObject(DeferredRenderObject* ro);
		// Resizes all registered FBOs
		void OnReSize(uint32 width, uint32 height);
		// Releases the FBOs from the GPU and CPU
		void Clean();

	};

}


