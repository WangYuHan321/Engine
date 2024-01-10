#pragma once
#include "../Util/common.h"

#include "glm/glm.hpp"
#include "../Render/IRenderable.h"
#include "../Render/Light/Light.h"
#include "../Util/Singleton.h"

namespace Engine
{
	class Setting
	{
	public:
		static uint32 worldSpeed;
		static uint32 drawingMethod;

		static glm::vec3 waterColor;
		static float waterSpeed;
		static float waterHeight;

		static float lightFactor;
		static glm::vec3 lightColor;
		static glm::vec3 realLightColor;
		static glm::vec3 lightDirection;

		static glm::vec3 skyZenitColor;
		static glm::vec3 skyHorizonColor;

		static float grassCoverage;
		static glm::vec3 grassColor;
		static glm::vec3 sandColor;
		static glm::vec3 rockColor;

		static float worldTileScale;
		static uint32 worldRenderRadius;

		static float terrainAmplitude;
		static float terrainFrecuency;
		static float terrainScale;
		static unsigned int terrainOctaves;

		static glm::vec3 windDirection;
		static float windStrength;

		static unsigned int drawClouds;
		static float cloudType;
		static float coverageMultiplier;
		static float innerSphereRadius;
		static float outerSphereRadius;
		static float sphereYOffset;
		static float cloudMaxRenderDistance;
		static float cloudTopOffset;
		static float weatherTextureScale;
		static float baseNoiseScale;
		static float highFrequencyNoiseScale;
		static float highFrequencyNoiseUVScale;
		static float highFrequencyNoiseHScale;
		static glm::vec3 cloudColor;

		static float godRaysDecay;
		static float godRaysDensity;
		static float godRaysExposure;
		static float godRaysWeight;


		static bool showUI;

		static uint32 debugIndex;

	public:
		Setting();
		~Setting();

		static void Update();
	};


}