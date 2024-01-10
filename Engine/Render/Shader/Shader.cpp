#include "Shader.h"
#include "../../Util/IOUtil.h"

const size_t VERSION_HEADER_LENGHT = 17;

Engine::Shader::~Shader() {}

Engine::uint32 Engine::Shader::GetShaderID() const
{
	return glProgramID;
}

Engine::Shader::Shader(const string& vShaderPath, const string& fShaderPath)
{
	vShaderFilePath = vShaderPath;
	fShaderFilePath = fShaderPath;
	gShaderFilePath = "";
	cShaderFilePath = "";
	gShader = -1;
	cShader = -1;

	glProgramID = glCreateProgram();

	uint32 vs = LoadShader(vShaderFilePath, GL_VERTEX_SHADER, "");
	uint32 fs = LoadShader(fShaderFilePath, GL_FRAGMENT_SHADER, "");

	glAttachShader(glProgramID, vs);
	glAttachShader(glProgramID, fs);
	glLinkProgram(glProgramID);

	int linked;
	glGetProgramiv(glProgramID, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		GLint logLen;
		glGetProgramiv(glProgramID, GL_INFO_LOG_LENGTH, &logLen);
		char* logString = new char[logLen];
		glGetProgramInfoLog(glProgramID, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete[] logString;
		exit(-1);
	}
}

Engine::Shader::Shader(const string& cShaderPath)
{
	vShaderFilePath = "";
	fShaderFilePath = "";
	gShaderFilePath = "";
	cShaderFilePath = cShaderPath;
	vShader = -1;
	fShader = -1;
	gShader = -1;

	glProgramID = glCreateProgram();

	uint32 cs = LoadShader(cShaderFilePath, GL_COMPUTE_SHADER, "");

	glAttachShader(glProgramID, cs);
	glLinkProgram(glProgramID);

	int linked;
	glGetProgramiv(glProgramID, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		GLint logLen;
		glGetProgramiv(glProgramID, GL_INFO_LOG_LENGTH, &logLen);
		char* logString = new char[logLen];
		glGetProgramInfoLog(glProgramID, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete[] logString;
		exit(-1);
	}
}

Engine::Shader::Shader(const string& vShaderPath, const string& fShaderPath, string configString)
{
	vShaderFilePath = vShaderPath;
	fShaderFilePath = fShaderPath;
	gShaderFilePath = "";
	cShaderFilePath = "";
	gShader = -1;
	cShader = -1;

	glProgramID = glCreateProgram();

	uint32 vs = LoadShader(vShaderFilePath, GL_VERTEX_SHADER, configString);
	uint32 fs = LoadShader(fShaderFilePath, GL_FRAGMENT_SHADER, configString);

	glAttachShader(glProgramID, vs);
	glAttachShader(glProgramID, fs);
	glLinkProgram(glProgramID);
	
	int linked;
	glGetProgramiv(glProgramID, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		GLint logLen;
		glGetProgramiv(glProgramID, GL_INFO_LOG_LENGTH, &logLen);
		char* logString = new char[logLen];
		glGetProgramInfoLog(glProgramID, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete[] logString;
		exit(-1);
	}

}

Engine::Shader::Shader(const string& vShaderPath, const string& gShaderPath, const string& fShaderPath, string configString)
{
	vShaderFilePath = vShaderPath;
	fShaderFilePath = fShaderPath;
	gShaderFilePath = gShaderPath;

	cShaderFilePath = "";
	cShader = -1;

	glProgramID = glCreateProgram();

	uint32 vs = LoadShader(vShaderFilePath, GL_VERTEX_SHADER, configString);
	uint32 fs = LoadShader(fShaderFilePath, GL_FRAGMENT_SHADER, configString);
	uint32 gs = LoadShader(gShaderFilePath, GL_GEOMETRY_SHADER, configString);

	glAttachShader(glProgramID, vs);
	glAttachShader(glProgramID, fs);
	glAttachShader(glProgramID, gs);
	glLinkProgram(glProgramID);

	int linked;
	glGetProgramiv(glProgramID, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		GLint logLen;
		glGetProgramiv(glProgramID, GL_INFO_LOG_LENGTH, &logLen);
		char* logString = new char[logLen];
		glGetProgramInfoLog(glProgramID, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete[] logString;
		exit(-1);
	}
}

Engine::Shader::Shader(const string& vShaderPath, const string& gShaderPath, const string& cShaderPath, const string& fShaderPath, string configString)
{
	vShaderFilePath = vShaderPath;
	fShaderFilePath = fShaderPath;
	gShaderFilePath = gShaderPath;
	cShaderFilePath = cShaderPath;

	glProgramID = glCreateProgram();

	uint32 vs = LoadShader(vShaderFilePath, GL_VERTEX_SHADER, configString);
	uint32 fs = LoadShader(fShaderFilePath, GL_FRAGMENT_SHADER, configString);
	uint32 gs = LoadShader(gShaderFilePath, GL_GEOMETRY_SHADER, configString);
	uint32 cs = LoadShader(cShaderFilePath, GL_COMPUTE_SHADER, configString);

	glAttachShader(glProgramID, vs);
	glAttachShader(glProgramID, fs);
	glAttachShader(glProgramID, gs);
	glAttachShader(glProgramID, cs);
	glLinkProgram(glProgramID);

	int linked;
	glGetProgramiv(glProgramID, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		GLint logLen;
		glGetProgramiv(glProgramID, GL_INFO_LOG_LENGTH, &logLen);
		char* logString = new char[logLen];
		glGetProgramInfoLog(glProgramID, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete[] logString;
		exit(-1);
	}

}

Engine::Shader::Shader(const string& vShaderPath, const string& tcShaderPath, const string& teShaderPath, const string& gShaderPath, const string& fShaderPath, string configString)
{
	vShaderFilePath = vShaderPath;
	fShaderFilePath = fShaderPath;
	gShaderFilePath = gShaderPath;
	tcShaderFilePath = tcShaderPath;
	teShaderFilePath = teShaderPath;

	glProgramID = glCreateProgram();

	uint32 vs = LoadShader(vShaderFilePath, GL_VERTEX_SHADER, configString);
	uint32 fs = LoadShader(fShaderFilePath, GL_FRAGMENT_SHADER, configString);
	uint32 tc = LoadShader(tcShaderFilePath, GL_TESS_CONTROL_SHADER, configString);
	uint32 te = LoadShader(teShaderFilePath, GL_TESS_EVALUATION_SHADER, configString);
	uint32 gs = LoadShader(gShaderFilePath, GL_GEOMETRY_SHADER, configString);

	glAttachShader(glProgramID, vs);
	glAttachShader(glProgramID, fs);
	glAttachShader(glProgramID, tc);
	glAttachShader(glProgramID, te);
	glAttachShader(glProgramID, gs);
	glLinkProgram(glProgramID);

	int linked;
	glGetProgramiv(glProgramID, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		GLint logLen;
		glGetProgramiv(glProgramID, GL_INFO_LOG_LENGTH, &logLen);
		char* logString = new char[logLen];
		glGetProgramInfoLog(glProgramID, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete[] logString;
		exit(-1);
	}
}

void Engine::Shader::Use()
{
	glUseProgram(glProgramID);
}

void Engine::Shader::Clean()
{

	if (vShaderFilePath != "")
	{
		glDetachShader(glProgramID, vShader);
		glDeleteShader(vShader);
	}

	if (fShaderFilePath != "")
	{
		glDetachShader(glProgramID, fShader);
		glDeleteShader(fShader);
	}


	if (gShaderFilePath != "")
	{
		glDetachShader(glProgramID, gShader);
		glDeleteShader(gShader);
	}

	if (cShaderFilePath != "")
	{
		glDetachShader(glProgramID, cShader);
		glDeleteShader(cShader);
	}

	glDeleteProgram(glProgramID);
}

void Engine::Shader::Dispatch(uint32 xDim, uint32 yDim, uint32 zDim, uint32 barrier)
{
	glDispatchCompute(xDim, yDim, zDim);
	glMemoryBarrier(barrier);
}


Engine::uint32 Engine::Shader::LoadShader(std::string fileName, GLenum type, string configString)
{
	size_t fileLen;

	char* source = Engine::IO::loadStringFromFile(fileName.c_str(), fileLen);

	std::string result(source);


	if (!configString.empty())
	{
		std::string header = result.substr(0, VERSION_HEADER_LENGHT);
		std::string body = result.substr(VERSION_HEADER_LENGHT, result.size() - VERSION_HEADER_LENGHT);
		result = header + "\n" + configString + "\n" + body;
	}

	char* finalSourceCStr = new char[result.size()];
	memcpy(finalSourceCStr, result.c_str(), result.size() * sizeof(char));
	fileLen = result.size();

	GLuint shader;
	shader = glCreateShader(type);
	glShaderSource(shader, 1, (const GLchar**)&finalSourceCStr, (const GLint*)&fileLen);
	glCompileShader(shader);
	delete[] source;
	delete[] finalSourceCStr;

	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		GLint logLen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
		char* logString = new char[logLen];
		glGetShaderInfoLog(shader, logLen, NULL, logString);
		std::cout << "Error on " << fileName << ": " << logString << std::endl;
		delete[] logString;
		glDeleteShader(shader);
		exit(-1);
	}

	return shader;
}

Engine::Shader* Engine::ShaderFactory::CreateShader(const string& cShaderPath)
{
	return new Shader(cShaderPath);
}

Engine::Shader* Engine::ShaderFactory::CreateShader(const string& vShaderPath, const string& fShaderPath)
{
	return new Shader(vShaderPath, fShaderPath);
}

Engine::Shader* Engine::ShaderFactory::CreateShader(const string& vShaderPath, const string& fShaderPath, string configString)
{
	return new Shader(vShaderPath, fShaderPath, configString);
}

Engine::Shader* Engine::ShaderFactory::CreateShader(const string& vShaderPath, const string& fShaderPath, const string& gShaderPath, string configString)
{
	return new Shader(vShaderPath, fShaderPath, gShaderPath, configString);
}

Engine::Shader* Engine::ShaderFactory::CreateShader(const string& vShaderPath, const string& fShaderPath, const string& gShaderPath, const string& cShaderPath, string configString)
{
	return new Shader(vShaderPath, fShaderPath, gShaderPath, cShaderPath, configString);
}

Engine::Shader* Engine::ShaderFactory::CreateShader(const string& vShaderPath, const string& fShaderPath, const string& tcShaderPath, const string& teShaderPath, const string& gShaderPath, string configString)
{
	return new Shader(vShaderPath, fShaderPath, tcShaderPath, teShaderPath, gShaderPath, configString);
}

Engine::Shader* Engine::ShaderFactory::InstantiateShader(uint32 id, std::initializer_list<const string> lst)
{
	std::map<uint32, Shader*>::iterator it = cache.find(id);
	if (it != cache.end())
	{
		return it->second;
	}
	else
	{
		if (lst.size() != 0)
		{
			switch (lst.size())
			{
			case 1:
			{
				cache[id] = CreateShader(*lst.begin());
			}
				break;
			case 2:
			{
				cache[id] = CreateShader(*lst.begin(), *lst.end());
			}
			break;
			case 3:
			{
				std::initializer_list<const string>::iterator it = lst.begin();
				cache[id] = CreateShader(*lst.begin(), *(++it), *lst.end());
			}
			break;
			case 4:
			{
				std::initializer_list<const string>::iterator it = lst.begin();
				cache[id] = CreateShader(*lst.begin(), *(++it), *(++(++it)), *lst.end());
			}
			break;
			case 5:
			{
				std::initializer_list<const string>::iterator it = lst.begin();
				cache[id] = CreateShader(*lst.begin(), *(++it), *(++(++it)), *(++(++(++it))), *lst.end());
			}
			break;
			case 6:
			{
				std::initializer_list<const string>::iterator it = lst.begin();
				cache[id] = CreateShader(*lst.begin(), *(++it), *(++(++it)), *(++(++(++it))), *(++(++(++(++it)))), *lst.end());
			}
			default:
				cache[id] = nullptr;
			}

		}

		return cache[id];
	}
}

void Engine::ShaderFactory::Clean()
{
	std::map<uint32, Shader*>::iterator it = cache.begin();
	while (it != cache.end())
	{
		it->second->Clean();
		delete it->second;
		it++;
	}
}
