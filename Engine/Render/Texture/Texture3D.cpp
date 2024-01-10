#include "Texture3D.h"

Engine::Texture3D::Texture3D(string _name, uint32 _width, uint32 _height, uint32 _depth)
	:
	Engine::AbstractTexture(_name),
	width(_width),
	height(_height),
	depth(_depth)
{
	data = nullptr;
}

Engine::Texture3D::~Texture3D()
{
	if (data != nullptr)
	{
		delete[] data;
	}
}

const Engine::uchar* Engine::Texture3D::GetTexturePixels() const
{
	return data;
}

const Engine::uint32 Engine::Texture3D::GetWidth() const
{
	return width;
}

const Engine::uint32 Engine::Texture3D::GetHeight()const
{
	return height;
}

const Engine::uint32 Engine::Texture3D::GetDepth()const
{
	return depth;
}

void Engine::Texture3D::SetSize(uint32 _width, uint32 _height, uint32 _depth)
{
	width = _width;
	height = _height;
	depth = _depth;
}

void Engine::Texture3D::UpLoadTexture()
{
	glBindTexture(GL_TEXTURE_3D, textureID);
	glTexStorage3D(GL_TEXTURE_3D, 6, internalFormat, width, height, depth);
	//glTexImage3D(GL_TEXTURE_3D, 0, internalFormat, width, height, depth, 0, formatType, pixelType, data);
	if (generatedMipMaps)
		glGenerateMipmap(GL_TEXTURE_3D);

	if (data != nullptr)
		delete[] data;
}

GLenum Engine::Texture3D::GetTextureType()
{
	return GL_TEXTURE_3D;
}