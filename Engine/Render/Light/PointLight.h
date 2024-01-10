#pragma once

#include "Light.h"

namespace Engine
{
	typedef struct PointLightData
	{
		float postion[4];//Î»ÖÃ
		float color[4];//ÑÕÉ«
		float attenuation[4]; //0 constant 1 lineal 2 cuadratic
		float kFactors[4]; //0=Ka 1= kd 2= ks
	}PointLightData;


	class PointLight : public Light
	{
	private:
		PointLightData shaderData;

	public:

		PointLight(string name);
		~PointLight();

		void SetPosition(const glm::vec3& pos);
		void SetAttenuation(const glm::vec3& att);
		void SetColor(const glm::vec3& color);
		void SetKa(float a);
		void SetKd(float d);
		void SetKs(float s);
		void Translate(const glm::vec3& translation);

		inline PointLightData& GetData() {
			return shaderData
				;
		}
	};
}


