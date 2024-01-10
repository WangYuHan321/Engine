#include "DebugPorgram.h"
#include "../../Config/WorldConfig.h"

Engine::string Engine::DebugPorgram::PROGRAM_NAME = "DebugPorgram";

Engine::DebugPorgram::DebugPorgram(std::string name, unsigned long long params):
	Program(name, params)
{
	vShaderFile = "shader/postprocess/PostProcessRender.vert";
	fShaderFile = "shader/postprocess/DebugPostProcess.frag";
}

Engine::DebugPorgram::DebugPorgram(unsigned long long params)
	: Program(PROGRAM_NAME, params)
{
	vShaderFile = "shader/postprocess/PostProcessRender.vert";
	fShaderFile = "shader/postprocess/DebugPostProcess.frag";
}

Engine::DebugPorgram::DebugPorgram(const DebugPorgram& other)
	: Program(other)
{
	planeVAO = other.planeVAO;
	planeVerticesVBO = other.planeVerticesVBO;

	inPos = other.inPos;
	inTexCoord = other.inTexCoord;

	uRenderedTexture = other.uRenderedTexture;
}

Engine::DebugPorgram::~DebugPorgram()
{
}

void Engine::DebugPorgram::ConfigureProgram()
{
	uRenderedTexture = glGetUniformLocation(shaderPtr->GetShaderID(), "postProcessing_0");
	inPos = glGetAttribLocation(shaderPtr->GetShaderID(), "inPos");
	inTexCoord = glGetAttribLocation(shaderPtr->GetShaderID(), "inUV");
}
void Engine::DebugPorgram::ConfigureMeshBuffer(Mesh* mesh)
{
	mesh->use();

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vboVertices);
	glVertexAttribPointer(inPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(inPos);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vboUVs);
	glVertexAttribPointer(inTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(inTexCoord);
}

void Engine::DebugPorgram::OnRenderObject(const Object* obj, Camera* cam)
{
	glUniform1i(uRenderedTexture, 0);//设置采样器索引
	glActiveTexture(GL_TEXTURE0);
	string colorName = "color_" + std::to_string(Engine::Setting::debugIndex);
	glBindTexture(GL_TEXTURE_2D, ((PostProcessObject*)obj)->GetTextureByName(colorName)->GetTexture()->GetTextureID());
}

Engine::Program* Engine::DebugProgramFactory::CreateProgram(Engine::ullong params)
{
	Engine::DebugPorgram* program = new Engine::DebugPorgram(Engine::DebugPorgram::PROGRAM_NAME, params);
	program->Initialize();
	return program;
}