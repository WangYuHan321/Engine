#include "AbstractTexture.h"

Engine::AbstractTexture::AbstractTexture(string _name) 
	:name(_name)
{
}

const Engine::uint32 Engine::AbstractTexture::GetTextureID() const
{
	return textureID;
}

void Engine::AbstractTexture::SetMemoryLayoutFormat(const int format)
{
	internalFormat = format;
}

void Engine::AbstractTexture::SetImageFormatType(const GLenum type)
{
	formatType = type;
}

void Engine::AbstractTexture::SetPixelFormatType(const GLenum type)
{
	pixelType = type;
}

void Engine::AbstractTexture::SetGenerateMipMaps(bool val)
{
	generatedMipMaps = val;
}

void Engine::AbstractTexture::GenerateTexture()
{
	glGenTextures(1, &textureID);
}


