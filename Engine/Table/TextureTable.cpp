#include "TextureTable.h"

Engine::TextureTable::~TextureTable()
{
	Clean();
}

void Engine::TextureTable::CacheTexture(string path, string name)
{

}

Engine::AbstractTexture* Engine::TextureTable::GetTextureByName(string name, std::initializer_list<const string> lst)
{
	return table[name];
}

void Engine::TextureTable::Clean()
{

}