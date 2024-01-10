#pragma once

#include "AbstractTexture.h"

namespace Engine
{
	class TextureInstance
	{
	private:
		AbstractTexture* texture;

		uint32 minificationFilter;
		uint32 magificationFilter;

		uint32 tComponentWrapType;
		uint32 sComponentWrapType;
		uint32 rComponentWrapType;

		bool applyAnisotropicFilteing;

	public:
		TextureInstance(AbstractTexture* texture);
		~TextureInstance();

		const AbstractTexture* GetTexture()const;

		void GenerateTexture();

		void UpLoadTexture();

		void ConfigureTexture() const;

		void GenerateMipMap();

		void SetMinificationFilterType(const int min);
	
		void SetMagnificationFilterType(const int mag);
		
		void SetTComponentWrapType(const int wrapT);
		
		void SetSComponentWrapType(const int wrapS);
	
		void SetRComponentWrapType(const int wrapR);

		//void SetAnisotropicFilterEnabled(bool val);

		const int GetMificationFilterType() const;
		const int GetMagnificationFilterType() const;
		const int GetTWrapType() const;
		const int GetSWrapType() const;
		const int GetRWrapType() const;
		//const bool isAnisotropicFilteringEnabled() const;

		void ReSize(uint32 w, uint32 h);
	};
}



