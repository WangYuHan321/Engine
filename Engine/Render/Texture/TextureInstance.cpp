#include "TextureInstance.h"

Engine::TextureInstance::TextureInstance(Engine::AbstractTexture* tture):
	texture(tture)
{
	minificationFilter = GL_LINEAR_MIPMAP_LINEAR;
	magificationFilter = GL_LINEAR;
	tComponentWrapType = GL_CLAMP_TO_EDGE;
	sComponentWrapType = GL_CLAMP_TO_EDGE;
	rComponentWrapType = GL_CLAMP_TO_EDGE;


}

Engine::TextureInstance::~TextureInstance()
{

}

const Engine::AbstractTexture* Engine::TextureInstance::GetTexture() const
{
	return texture;
}

void Engine::TextureInstance::GenerateTexture()
{
	texture->GenerateTexture();
}

void Engine::TextureInstance::UpLoadTexture()
{
	texture->UpLoadTexture();
}

void Engine::TextureInstance::SetMinificationFilterType(const int min)
{
	minificationFilter = min;
}

void Engine::TextureInstance::SetMagnificationFilterType(const int mag)
{
	magificationFilter = mag;
}

void Engine::TextureInstance::SetTComponentWrapType(const int wrapT)
{
	tComponentWrapType = wrapT;
}

void Engine::TextureInstance::SetSComponentWrapType(const int wrapS)
{
	sComponentWrapType = wrapS;
}

void Engine::TextureInstance::SetRComponentWrapType(const int wrapR)
{
	rComponentWrapType = wrapR;
}

//void Engine::TextureInstance::SetAnisotropicFilterEnabled(bool val)
//{
//	applyAnisotropicFiltering = applyAnisotropicFiltering && val;
//}

const int Engine::TextureInstance::GetMificationFilterType() const
{
	return minificationFilter;
}

const int Engine::TextureInstance::GetMagnificationFilterType() const
{
	return magificationFilter;
}

const int Engine::TextureInstance::GetTWrapType() const
{
	return tComponentWrapType;
}

const int Engine::TextureInstance::GetSWrapType() const
{
	return sComponentWrapType;
}

const int Engine::TextureInstance::GetRWrapType() const
{
	return rComponentWrapType;
}

//const bool Engine::TextureInstance::isAnisotropicFilteringEnabled() const
//{
//	return applyAnisotropicFiltering;
//}

void Engine::TextureInstance::ReSize(unsigned int w, unsigned int h)
{
	texture->SetSize(w, h);
	texture->UpLoadTexture();
	ConfigureTexture();
}

void Engine::TextureInstance::ConfigureTexture() const
{
	GLenum type = texture->GetTextureType();

	glBindTexture(type, texture->GetTextureID());
	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, minificationFilter);
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, magificationFilter);
	glTexParameteri(type, GL_TEXTURE_WRAP_T, tComponentWrapType);
	glTexParameteri(type, GL_TEXTURE_WRAP_S, sComponentWrapType);
	glTexParameteri(type, GL_TEXTURE_WRAP_R, rComponentWrapType);

}

void Engine::TextureInstance::GenerateMipMap()
{
	glBindTexture(texture->GetTextureType(), texture->GetTextureID());
	glGenerateMipmap(texture->GetTextureType());
}