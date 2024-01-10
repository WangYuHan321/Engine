#pragma once

#include "AbstractTexture.h"

namespace Engine
{

	class Texture2D : public AbstractTexture
	{
	private:
		uchar* data;
		uint32 width;
		uint32 height;

	public:
		Texture2D(string name, uchar* data, uint32 width, uint32 height);
		~Texture2D();

		const uchar* genTexturePixels() const;
		const uint32 getWidth() const;
		const uint32 getHeight()const;

		virtual void SetSize(uint32 _width,uint32 _height, uint32 _depth = 1) override;
		virtual void UpLoadTexture() override;
		virtual GLenum GetTextureType() override;

	};

}


