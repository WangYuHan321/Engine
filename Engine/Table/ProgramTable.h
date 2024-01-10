#pragma once

#include "StorageTable.h"
#include "../Render/Program/Program.h"

namespace Engine
{
	class ProgramTable :public CSingleton<ProgramTable>,
		public StorageTable
	{
	private:
		std::map<std::string, ProgramFactory*> table;

	public:

		~ProgramTable();
		void RegisterProgramFactory(string name, ProgramFactory* pf);
		Program* GetProgramByName(string name, ullong parameters = 0);

		template< typename T>
		T* GetProgram(ullong parameters = 0)
		{
			Program* p = GetProgramByName(T::PROGRAM_NAME, parameters);
			if (p == nullptr) return nullptr;
			return static_cast<T*>(p);
		}

		void Clean();

	};
}


