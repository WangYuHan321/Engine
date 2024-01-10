#include "Texture2D.h"
Engine::Texture2D::Texture2D(string _name, uchar* _data, uint32 _width, uint32 _height)
	:
	Engine::AbstractTexture(_name),
	width(_width),
	height(_height)
{
	if (data != nullptr)
	{
		data = new uchar[width * height];
		memcpy(this->data, _data, width * height * sizeof(uchar));
	}

}

Engine::Texture2D::~Texture2D()
{
	if (data != nullptr)
	{
		delete[] data;
	}
}

const Engine::uchar* Engine::Texture2D::genTexturePixels() const
{
	return data;
}

const Engine::uint32 Engine::Texture2D::getWidth() const
{
	return width;
}

const Engine::uint32 Engine::Texture2D::getHeight()const
{
	return height;
}

void Engine::Texture2D::SetSize(uint32 _width, uint32 _height, uint32 _depth)
{
	width = _width;
	height = _height;
}

void Engine::Texture2D::UpLoadTexture()
{
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, formatType, pixelType, data);
	if (generatedMipMaps)
		glGenerateMipmap(GL_TEXTURE_2D);

	if (data != nullptr)
		delete[] data;
}

GLenum Engine::Texture2D::GetTextureType()
{
	return GL_TEXTURE_2D;
}