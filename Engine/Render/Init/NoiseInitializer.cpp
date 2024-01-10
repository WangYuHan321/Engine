#include "NoiseInitializer.h"
#include "../Texture/Texture3D.h"
#include "../Texture/Texture2D.h"


void Engine::CloudSystem::NoiseInitializer::Init()
{
	if (initialized)
		return;

	InitPrograms();
	InitTextures();

	initialized = true;
}

void Engine::CloudSystem::NoiseInitializer::InitPrograms()
{
	perlinWorleyGen = new Engine::VolumeTextureProgram("shader/clouds/generation/perlinworley.comp");
	perlinWorleyGen->Initialize();
	worleyGen = new Engine::VolumeTextureProgram("shader/clouds/generation/worley.comp");
	worleyGen->Initialize();
	weatherGen = new Engine::WeatherTextureProgram();
	weatherGen->Initialize();
}

void Engine::CloudSystem::NoiseInitializer::Render()
{
	perlinWorleyGen->Use();
	perlinWorleyGen->BindOutput(PerlinWorleyFBM);
	perlinWorleyGen->Dispatch(128, 128, 128, GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	glBindTexture(GL_TEXTURE_3D, PerlinWorleyFBM->GetTexture()->GetTextureID());
	glGenerateMipmap(GL_TEXTURE_3D);
	PerlinWorleyFBM->ConfigureTexture();

	worleyGen->Use();
	worleyGen->BindOutput(WorleyFBM);
	worleyGen->Dispatch(32, 32, 32, GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	glBindTexture(GL_TEXTURE_3D, WorleyFBM->GetTexture()->GetTextureID());
	glGenerateMipmap(GL_TEXTURE_3D);
	WorleyFBM->ConfigureTexture();

	weatherGen->Use();
	weatherGen->BindOutput(WeatherData);
	weatherGen->Dispatch(2048, 2048, 1, GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

void Engine::CloudSystem::NoiseInitializer::InitTextures()
{
	Engine::Texture3D* perlinWorley = new Engine::Texture3D("perlinworleyfbm", 128, 128, 128);
	perlinWorley->SetGenerateMipMaps(false);
	perlinWorley->SetMemoryLayoutFormat(GL_RGBA8);
	perlinWorley->SetImageFormatType(GL_RGBA);
	perlinWorley->SetPixelFormatType(GL_FLOAT);

	PerlinWorleyFBM = new Engine::TextureInstance(perlinWorley);
	PerlinWorleyFBM->SetRComponentWrapType(GL_REPEAT);
	PerlinWorleyFBM->SetSComponentWrapType(GL_REPEAT);
	PerlinWorleyFBM->SetTComponentWrapType(GL_REPEAT);
	PerlinWorleyFBM->SetMagnificationFilterType(GL_NEAREST);
	PerlinWorleyFBM->SetMinificationFilterType(GL_NEAREST);
	PerlinWorleyFBM->GenerateTexture();
	PerlinWorleyFBM->UpLoadTexture();
	PerlinWorleyFBM->ConfigureTexture();

	Engine::Texture3D* worley = new Engine::Texture3D("worleyfbm", 32, 32, 32);
	worley->SetGenerateMipMaps(false);
	worley->SetMemoryLayoutFormat(GL_RGBA8);
	worley->SetImageFormatType(GL_RGBA);
	worley->SetPixelFormatType(GL_FLOAT);

	WorleyFBM = new Engine::TextureInstance(worley);
	WorleyFBM->SetRComponentWrapType(GL_REPEAT);
	WorleyFBM->SetSComponentWrapType(GL_REPEAT);
	WorleyFBM->SetTComponentWrapType(GL_REPEAT);
	WorleyFBM->SetMagnificationFilterType(GL_LINEAR);
	WorleyFBM->SetMinificationFilterType(GL_LINEAR);
	WorleyFBM->GenerateTexture();
	WorleyFBM->UpLoadTexture();
	WorleyFBM->ConfigureTexture();

	Engine::Texture2D* weather = new Engine::Texture2D("weather", 0, 2048, 2048);
	weather->SetGenerateMipMaps(false);
	weather->SetMemoryLayoutFormat(GL_RGBA8);
	weather->SetImageFormatType(GL_RGBA);
	weather->SetPixelFormatType(GL_FLOAT);

	WeatherData = new Engine::TextureInstance(weather);
	WeatherData->SetSComponentWrapType(GL_MIRRORED_REPEAT);
	WeatherData->SetTComponentWrapType(GL_MIRRORED_REPEAT);
	WeatherData->SetMagnificationFilterType(GL_LINEAR);
	WeatherData->SetMinificationFilterType(GL_LINEAR);
	WeatherData->GenerateTexture();
	WeatherData->UpLoadTexture();
	WeatherData->ConfigureTexture();
}

void Engine::CloudSystem::NoiseInitializer::Clean()
{
	if (perlinWorleyGen = nullptr)
	{
		perlinWorleyGen->Destroy();
		delete perlinWorleyGen;
	}

	if (worleyGen = nullptr)
	{
		worleyGen->Destroy();
		delete worleyGen;
	}
}