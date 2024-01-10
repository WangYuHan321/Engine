#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include "../../Util/common.h"

namespace Engine
{
	class Shader
	{
	private:
		uint32 glProgramID;
		uint32 vShader;
		uint32 fShader;
		uint32 gShader;
		uint32 cShader;

		string vShaderFilePath;
		string fShaderFilePath;
		string gShaderFilePath;
		string cShaderFilePath;

		string tcShaderFilePath = "";
		string teShaderFilePath = "";

	public:
		Shader() = delete;
		Shader(const string& cShaderPath);
		Shader(const string& vShaderPath, const string& fShaderPath);
		Shader(const string& vShaderPath, const string& fShaderPath, string configString);
		Shader(const string& vShaderPath, const string& fShaderPath, const string& gShaderPath, string configString);
		Shader(const string& vShaderPath, const string& fShaderPath, const string& gShaderPath, const string& cShaderPath, string configString);
		Shader(const string& vShaderPath, const string& fShaderPath, const string& tcShaderPath, const string& teShaderPath, const string& gShaderPath, string configString);
		~Shader();

		uint32 GetShaderID() const ;

		void Use();
		void Clean();
		void Dispatch(uint32 xDim, uint32 yDim, uint32 zDim, uint32 barrier);
		uint32 LoadShader(std::string fileName, GLenum type, string configString = "");
	};

	class ShaderFactory
	{
	private:
		std::map<uint32, Shader*> cache;
	public:
		Shader* CreateShader(const string& cShaderPath);
		Shader* CreateShader(const string& vShaderPath, const string& fShaderPath);
		Shader* CreateShader(const string& vShaderPath, const string& fShaderPath, string configString);
		Shader* CreateShader(const string& vShaderPath, const string& fShaderPath, const string& gShaderPath, string configString);
		Shader* CreateShader(const string& vShaderPath, const string& fShaderPath, const string& gShaderPath, const string& cShaderPath, string configString);
		Shader* CreateShader(const string& vShaderPath, const string& fShaderPath, const string& tcShaderPath, const string& teShaderPath, const string& gShaderPath, string configString);

		Shader* InstantiateShader(uint32 id, std::initializer_list<const string> lst = {});

		void Clean();
	};
}



