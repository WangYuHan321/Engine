#include "WorldConfig.h"

Engine::uint32 Engine::Setting::worldSpeed = 0;
Engine::uint32 Engine::Setting::drawingMethod = 0;

glm::vec3 Engine::Setting::lightDirection = glm::vec3(1, 1, 0);

glm::vec3 Engine::Setting::waterColor = glm::vec3(0.06f, 0.52f, 0.337f);
float Engine::Setting::waterSpeed = 0.0025f;
float Engine::Setting::waterHeight = 0.09f;


float Engine::Setting::lightFactor = 1.0f;
glm::vec3 Engine::Setting::lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 Engine::Setting::realLightColor = glm::vec3(1.0f, 1.0f, 1.0f);

glm::vec3 Engine::Setting::skyZenitColor = glm::vec3(0.0f, 0.5f, 1.0f);
glm::vec3 Engine::Setting::skyHorizonColor = glm::vec3(0.8f, 0.85f, 1.0f);

float Engine::Setting::worldTileScale = 7.0f;
Engine::uint32 Engine::Setting::worldRenderRadius = 12;

float Engine::Setting::terrainAmplitude = 0.732f;
float Engine::Setting::terrainFrecuency = 0.244f;
float Engine::Setting::terrainScale = 0.9f;
unsigned int Engine::Setting::terrainOctaves = 10;

float Engine::Setting::grassCoverage = 0.5f;
glm::vec3 Engine::Setting::grassColor = glm::vec3(0.1f, 0.3f, 0.0f);
glm::vec3 Engine::Setting::sandColor = glm::vec3(0.94f, 0.89f, 0.5f);
glm::vec3 Engine::Setting::rockColor = glm::vec3(0.40f);


glm::vec3 Engine::Setting::windDirection = glm::vec3(1, 0, 0);
float Engine::Setting::windStrength = 0.5f;

unsigned int Engine::Setting::drawClouds = 0;
float Engine::Setting::cloudType = 0.5f;
float Engine::Setting::coverageMultiplier = 1.0f;
float Engine::Setting::innerSphereRadius = 10000000.0f * 2.0;
float Engine::Setting::outerSphereRadius = 10900000.0f * 2.0;
float Engine::Setting::sphereYOffset = -9600000.0f * 2.0;
float Engine::Setting::cloudMaxRenderDistance = 6000000.0f;
float Engine::Setting::cloudTopOffset = 0.0f;
float Engine::Setting::weatherTextureScale = 1.0f;
float Engine::Setting::baseNoiseScale = 1.0f;
float Engine::Setting::highFrequencyNoiseScale = 1.0f;
float Engine::Setting::highFrequencyNoiseUVScale = 150.0f;
float Engine::Setting::highFrequencyNoiseHScale = 4.0f;
glm::vec3 Engine::Setting::cloudColor = glm::vec3(1, 1, 1);

float Engine::Setting::godRaysDecay = 0.904f;
float Engine::Setting::godRaysDensity = 0.318f;
float Engine::Setting::godRaysExposure = 0.515f;
float Engine::Setting::godRaysWeight = 0.2f;


bool Engine::Setting::showUI = false;

Engine::uint32 Engine::Setting::debugIndex = 0;

Engine::Setting::Setting()
{
}

Engine::Setting::~Setting()
{}

void Engine::Setting::Update()
{
	lightFactor = glm::clamp(glm::dot(glm::vec3(0, 1, 0), glm::normalize(lightDirection)), 0.0f, 1.0f);
	realLightColor = lightColor;
	realLightColor.y *= lightFactor * 0.85f;
	realLightColor.z *= lightFactor * 0.55f;
}

