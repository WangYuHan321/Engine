#pragma once

#include "../../Util/Singleton.h"
#include "../Texture/TextureInstance.h"
#include "../Program/ComputeProgram/VolumeTextureProgram.h"
#include "../Program/ComputeProgram/WeatherTextureProgram.h"

namespace Engine
{
	namespace CloudSystem
	{

		/**
		* 负责填充云的噪声体积纹理的类
		* 以及对它生成天气数据纹理并授予访问权限
		*/

		class NoiseInitializer : public CSingleton<NoiseInitializer>
		{

		private:
			// 我们可以寻求更多资源，但看起来像这样就很好
			
			// r = Perlin-Worley FBM X octave
			// g = Worley X + 1 octave
			// b = Worley X + 2 octave
			// c = Worley X + 3 octave
			// res 128 * 128 * 128

			TextureInstance* PerlinWorleyFBM;

			// r = Worley X octave
			// g = Worley X + 1 octave
			// b = Worley X + 2 octave
			// res 32 * 32 * 32
			TextureInstance* WorleyFBM;

			//r = 云覆盖（Clamped Perlin：如果 perling < 0.7，为 0，否则为 perlin）
			//g = 雨
			//b = 云类型
			//res = 1024 * 1024 
			TextureInstance* WeatherData;


			//着色器从 GPU 写入纹理（CPU 生成速度会很慢）
			VolumeTextureProgram* perlinWorleyGen;
			VolumeTextureProgram* worleyGen;
			WeatherTextureProgram* weatherGen;

			bool initialized;

		public:
			inline const TextureInstance* GetPerlinWorleyFBM() const {return  PerlinWorleyFBM; }
			inline const TextureInstance* GetWorleyFBM() const { return WorleyFBM; }
			inline const TextureInstance* GetWeatherData() const { return WeatherData; }

			void Render();

		public:
			
			void Init();
			void InitPrograms();
			void InitTextures();

			void Clean();
		};
	}
}



