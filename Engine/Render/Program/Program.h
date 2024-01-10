#pragma once

#include "../Shader/Shader.h"
#include "../Mesh/Mesh.h"
#include "../Camera/Camera.h"
#include "../Object/Object.h"

namespace Engine
{
	class Program
	{
	protected:
		Shader* shaderPtr;

		string vShaderFile;
		string fShaderFile;
		string gShaderFile;

		ullong paramters;

	private:
		string name;

	public:
		Program(string name, ullong params = 0);
		Program(const Program& other);
		~Program();

		inline string GetName() const { return name; }
		uint32 GetProgramID() const;
		virtual void Initialize();

		virtual void ConfigureProgram() = 0;
		virtual void ConfigureMeshBuffer(Mesh * mesh) = 0;

		void Use();
		virtual void ApplyGlobalUniforms();
		virtual void OnRenderObject(const Object* obj, Camera* cam) = 0;

		virtual void Destory();
	};


	class ProgramFactory
	{
	private:
		std::map<unsigned long long, Program*> cache;
	protected:
		virtual Program* CreateProgram(unsigned long long parameters) = 0;
	public:
		Program* InstantiateProgram(unsigned long long parameters);
		void Clean();
	};
}


