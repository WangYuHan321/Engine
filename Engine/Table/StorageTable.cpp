#include "StorageTable.h"

void Engine::TableManger::RegisterTable(StorageTable* table)
{
	tables.push_back(table);
}

void Engine::TableManger::Clean()
{
	for (auto item : tables)
		item->Clean();
	tables.clear();
}