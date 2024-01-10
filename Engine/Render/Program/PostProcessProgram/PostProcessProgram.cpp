#include "PostProcessProgram.h"
#include "../../Object/Object.h"

Engine::string Engine::PostProcessProgram::PROGRAM_NAME = "PostProcessProgram";

Engine::PostProcessProgram::PostProcessProgram(std::string name, unsigned long long params)
	: Engine::Program(name, params)
{
	vShaderFile = "shader/postprocess/PostProcessRender.vert";
	fShaderFile = "shader/postprocess/PostProcessRender.frag";
}

Engine::PostProcessProgram::PostProcessProgram(unsigned long long params)
	: Engine::PostProcessProgram(Engine::PostProcessProgram::PROGRAM_NAME, params)

{
}

Engine::PostProcessProgram::PostProcessProgram(const PostProcessProgram& other)
	: Engine::Program(other)
{
	planeVAO = other.planeVAO;
	planeVerticesVBO = other.planeVerticesVBO;

	inPos = other.inPos;
	inTexCoord = other.inTexCoord;

	memcpy(uRenderedTextures, other.uRenderedTextures, sizeof(uint32) * 9);
}

Engine::PostProcessProgram::~PostProcessProgram()
{

}

void Engine::PostProcessProgram::ConfigureProgram()
{
	for (int i = 0; i < 9; i++)
	{
		string uniformName = "postProcessing_" + std::to_string(i);
		uRenderedTextures[i] = glGetUniformLocation(shaderPtr->GetShaderID(), uniformName.c_str());
	}

	inPos = glGetAttribLocation(shaderPtr->GetShaderID(), "inPos");
	inTexCoord = glGetAttribLocation(shaderPtr->GetShaderID(), "inTexCoord");
}

void Engine::PostProcessProgram::ConfigureMeshBuffer(Mesh* mesh)
{
	mesh->use();

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vboVertices);
	glVertexAttribPointer(inPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(inPos);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vboUVs);
	glVertexAttribPointer(inTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(inTexCoord);
}

void Engine::PostProcessProgram::OnRenderObject(const Object* obj, Camera* cam)
{
	
	std::map<string, TextureInstance*> all = ((PostProcessObject*)obj)->GetAllCustomTextures();
	std::map<string, TextureInstance*>::const_iterator it = all.cbegin();

	int start = 0;
	while (it != all.cend() && start < 9)
	{
		if (uRenderedTextures[start] != -1)
		{
			glUniform1i(uRenderedTextures[start], start);
			glActiveTexture(GL_TEXTURE0 + start);
			glBindTexture(GL_TEXTURE_2D, it->second->GetTexture()->GetTextureID());
		}
		start++;
		it++;
	}
}

Engine::Program* Engine::PostProcessProgramFactory::CreateProgram(unsigned long long parameters)
{
	Engine::PostProcessProgram* program = new Engine::PostProcessProgram(Engine::PostProcessProgram::PROGRAM_NAME, parameters);
	program->Initialize();
	return program;
}