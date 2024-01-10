#pragma once

#include "../../Shader/Shader.h"
#include "../../Mesh/Mesh.h"
#include "../../Camera/Camera.h"
#include "../../Object/Object.h"

namespace Engine
{
	class ComputeProgram
	{
	protected:
		string computeShaderFile;
		Shader* shaderPtr;

	public:
		ComputeProgram(std::string shaderFile);
		ComputeProgram(const ComputeProgram& other);

		uint32 GetProgramID();
		void Use();

		void Initialize();
		virtual void ConfigureProgram() = 0;

		// Dispatch the compute shader with the given grid configuration, and stablishing the
		// synchronization barrier type
		void Dispatch(unsigned int xDim, unsigned int yDim, unsigned int zDim, unsigned int barrier);

		void Destroy();
	};
}


