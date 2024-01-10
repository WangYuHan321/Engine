#include "ShaderTable.h"

void Engine::ShaderTable::RegisterShaderFactory(string name, ShaderFactory* sf)
{
	table[name] = sf;
}

Engine::Shader* Engine::ShaderTable::GetShaderByName(string name, std::initializer_list<const string> lst)
{
	std::map<std::string, Engine::ShaderFactory*>::iterator it = table.find(name);

	if (it == table.end())
	{
		std::cout << "ShaderTable: Tried to access unexistent program " << name << ", " << "" << std::endl;
		return NULL;
	}
	else
	{
		Engine::ShaderFactory* factory = it->second;
		return factory->InstantiateShader(Hash(name.c_str()), lst);
	}
}

void Engine::ShaderTable::Clean()
{
	std::map<std::string, Engine::ShaderFactory*>::iterator it = table.begin();
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

Engine::ShaderTable::~ShaderTable()
{
	Clean();
}