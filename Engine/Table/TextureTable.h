#pragma once

#include"StorageTable.h"
#include "../Render/Texture/AbstractTexture.h"
#include "../Render/Shader/Shader.h"

namespace Engine 
{
	class TextureTable : public CSingleton<TextureTable>,
	public StorageTable
	{
	private:
		std::map<string, AbstractTexture*> table;

	public:

		~TextureTable();

		void CacheTexture(string path, string name);
		AbstractTexture* GetTextureByName(string name, std::initializer_list<const string> lst = {});

		void Clean();
	};
}


