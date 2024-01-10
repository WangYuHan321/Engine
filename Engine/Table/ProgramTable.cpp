#include "ProgramTable.h"


void Engine::ProgramTable::RegisterProgramFactory(string name, ProgramFactory* pf)
{
	if (pf != NULL)
	{
		table[name] = pf;
	}
}

Engine::Program* Engine::ProgramTable::GetProgramByName(string name, ullong parameters)
{
	std::map<std::string, Engine::ProgramFactory*>::iterator it = table.find(name);

	if (it == table.end())
	{
		std::cout << "ProgramTable: Tried to access unexistent program " << name << ", " << parameters << std::endl;
		return NULL;
	}
	else
	{
		Engine::ProgramFactory* factory = it->second;
		return factory->InstantiateProgram(parameters);
	}
}

void Engine::ProgramTable::Clean()
{
	std::map<std::string, Engine::ProgramFactory*>::iterator it = table.begin();
	while (it != table.end())
	{
		if (it->second != NULL)
		{
			it->second->Clean();
			delete it->second;
		}
		it++;
	}
	table.clear();
}

Engine::ProgramTable::~ProgramTable()
{
	Clean();
}