#pragma once

#include "Program.h"

namespace Engine
{
	class ProceduralWaterProgram : public Program
	{
	public:

		static string PROGRAM_NAME;

		static const ullong WIRE_DRAW_MODE;
		static const ullong POINT_DRAW_MODE;
		static const ullong SHADOW_MAP;

		string tctrlShaderFile;
		string tevalShaderFile;

	private:

		uint32 uInPos;
		uint32 uInUV;

		uint32 uModelView;
		uint32 uModelViewProj;
		uint32 uNormal;

		uint32 uGBuffer;
		uint32 uScreenSize;

		uint32 uInInfo;
		uint32 uGridPos;


		uint32 uLightDepthMatrix;
		uint32 uLightDepthMatrix1;

		uint32 uDepthTexture;
		uint32 uDepthTexture1;

		uint32 uLightDirection;

		//shadow

		uint32 uWorldGridPos;
		uint32 uTime;

		uint32 uWaterColor;
		uint32 uWaterSpeed;

	public:

		ProceduralWaterProgram(string name, ullong paramters);
		ProceduralWaterProgram(const ProceduralWaterProgram& program);

		virtual void Initialize() override;
		virtual void ConfigureProgram() override;
		virtual void ConfigureMeshBuffer(Mesh* mesh) override;

		virtual void ApplyGlobalUniforms() override;
		virtual void OnRenderObject(const Object* obj, Camera* cam) override;

		void SetUniformGridPosition(unsigned int i, unsigned int j);
		void SetUniformLightDepthMatrix(const glm::mat4& ldm);
		void SetUniformLightDepthMatrix1(const glm::mat4& ldm);
	};

	class ProceduralWaterProgramFactory : public ProgramFactory
	{
	protected:
		virtual Program* CreateProgram(ullong paramters) override;
	};
}



