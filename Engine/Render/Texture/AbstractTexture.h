#pragma once

#include "../../Util/common.h"
#include<GL/glew.h>

namespace Engine
{
	class AbstractTexture
	{
	protected:
		uint32 textureID;
		string name;

		int internalFormat;
		GLenum formatType;
		GLenum pixelType;
		bool generatedMipMaps;

	public:
		AbstractTexture(string name);
		const uint32 GetTextureID() const;

		void SetMemoryLayoutFormat(const int format);
		void SetImageFormatType(const GLenum type);
		void SetPixelFormatType(const GLenum type);
		void SetGenerateMipMaps(bool val);

		void GenerateTexture();

		virtual void UpLoadTexture() = 0;
		virtual void SetSize(unsigned int w, unsigned int h, unsigned int d = 1) = 0;
		virtual GLenum GetTextureType() = 0;

	};


}

