#pragma once

#include "Light.h"

namespace Engine 
{
	typedef struct DirectionalLightData
	{
		float direction[4];
		float color[4];
		float kFactors[4];
	}DirectionalLightData;

	class DirectionalLight : public Light
	{
	private:
		glm::vec3 direction;
		DirectionalLightData shaderData;
	public:
		DirectionalLight(std::string name = "");
		~DirectionalLight();

		const glm::vec3 GetDirection() const;

		// Will change light direction (directional lights have no position)
		void Translate(const glm::vec3& translation);

		// Setters
		void SetDirection(const glm::vec3& dir);
		void SetColor(const glm::vec3& color);
		void SetKa(float a);
		void SetKd(float d);
		void SetKs(float s);

		DirectionalLightData& GetData();
	};

}


