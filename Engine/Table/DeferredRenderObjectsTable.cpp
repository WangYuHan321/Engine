#include "DeferredRenderObjectsTable.h"

void Engine::DeferredRenderObjectsTable::RegisterDeferredObject(DeferredRenderObject* obj)
{
	deferredRenderObjects.push_back(obj);
}

void Engine::DeferredRenderObjectsTable::OnReSize(uint32 w, uint32 h)
{
	for (auto item : deferredRenderObjects)
	{
		item->ReSizeFBO(w, h);
	}
}

void Engine::DeferredRenderObjectsTable::Clean()
{
	for (auto item : deferredRenderObjects)
	{
		delete item;
	}

	deferredRenderObjects.clear();
}