#pragma once
#include "ComputeProgram.h"

namespace Engine
{
	class WeatherTextureProgram :
		public ComputeProgram
	{
	private:
		uint32 uWeatherTex;

	public:
		WeatherTextureProgram();
		WeatherTextureProgram(const WeatherTextureProgram& other);

		void ConfigureProgram();
		void BindOutput(const TextureInstance* ti);
	};
}


