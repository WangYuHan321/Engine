#pragma once

#include "AbstractTexture.h"

namespace Engine
{
	class Texture3D : public AbstractTexture
	{
	private:
		uchar* data;
		uint32 width;
		uint32 height;
		uint32 depth;

	public:

		Texture3D(string _name, uint32 _width, uint32 _height, uint32 _depth);
		~Texture3D();

		const uchar* GetTexturePixels() const;
		const uint32 GetWidth() const;
		const uint32 GetHeight()const;
		const uint32 GetDepth()const;

		virtual void SetSize(unsigned int w, unsigned int h, unsigned int d) override;
		virtual void UpLoadTexture() override;
		virtual GLenum GetTextureType() override;
	};

}



