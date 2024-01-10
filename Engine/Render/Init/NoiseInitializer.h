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
		* ��������Ƶ���������������
		* �Լ����������������������������Ȩ��
		*/

		class NoiseInitializer : public CSingleton<NoiseInitializer>
		{

		private:
			// ���ǿ���Ѱ�������Դ�����������������ͺܺ�
			
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

			//r = �Ƹ��ǣ�Clamped Perlin����� perling < 0.7��Ϊ 0������Ϊ perlin��
			//g = ��
			//b = ������
			//res = 1024 * 1024 
			TextureInstance* WeatherData;


			//��ɫ���� GPU д������CPU �����ٶȻ������
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



