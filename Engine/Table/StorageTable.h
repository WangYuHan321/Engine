#pragma once

#include "../Util/Singleton.h"
#include "../Util/common.h"

namespace Engine
{
	class StorageTable
	{
	public:
		virtual void Clean() = 0;
	};


	class TableManger : public CSingleton<TableManger>
	{
	private:
		std::list<StorageTable*> tables;

	public:

		void RegisterTable(StorageTable* table);
		void Clean();

	};
}


