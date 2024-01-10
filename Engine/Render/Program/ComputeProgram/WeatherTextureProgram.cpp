#include "WeatherTextureProgram.h"


Engine::WeatherTextureProgram::WeatherTextureProgram():
	Engine::ComputeProgram("shader/clouds/generation/weather.comp")
{
}

Engine::WeatherTextureProgram::WeatherTextureProgram(const WeatherTextureProgram& other)
	:Engine::ComputeProgram(other)
{
	uWeatherTex = other.uWeatherTex;
}

void Engine::WeatherTextureProgram::ConfigureProgram()
{
	uWeatherTex = glGetUniformLocation(shaderPtr->GetShaderID(), "outWeatherTex");
}

void Engine::WeatherTextureProgram::BindOutput(const TextureInstance* ti)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ti->GetTexture()->GetTextureID());
	glBindImageTexture(0, ti->GetTexture()->GetTextureID(), 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA8);
	glUniform1i(uWeatherTex, 0);
}
