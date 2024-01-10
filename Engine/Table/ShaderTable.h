#pragma once

#include<initializer_list> 
#include"StorageTable.h"
#include "../Render/Shader/Shader.h"

namespace Engine
{
	class ShaderTable : public StorageTable ,
		public CSingleton<ShaderTable>
	{
	private:
		std::map<string, ShaderFactory*> table;

	public:

		~ShaderTable();

		void RegisterShaderFactory(string name, ShaderFactory* sf);
		Shader* GetShaderByName(string name, std::initializer_list<const string> lst = {});

		void Clean();
	};
}


