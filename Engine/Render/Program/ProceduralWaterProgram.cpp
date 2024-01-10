#include "ProceduralWaterProgram.h"
#include "../../Config/WorldConfig.h"
#include "../../Util/Time.h"
#include "../DeferredRender/DeferredRenderer.h"
#include "../Shadow/CascadeShadowMaps.h"

const Engine::ullong Engine::ProceduralWaterProgram::WIRE_DRAW_MODE = 0x01;
const Engine::ullong Engine::ProceduralWaterProgram::POINT_DRAW_MODE = 0x02;
const Engine::ullong Engine::ProceduralWaterProgram::SHADOW_MAP = 0x04;

Engine::string Engine::ProceduralWaterProgram::PROGRAM_NAME = "ProceduralWaterProgram";

Engine::ProceduralWaterProgram::ProceduralWaterProgram(string name, ullong paramters)
	:Engine::Program(name, paramters)
{
	vShaderFile = "shader/water/water.vert";
	tctrlShaderFile = "shader/water/water.tesctrl";
	tevalShaderFile = "shader/water/water.teseval";
	gShaderFile = "shader/water/water.geom";
	fShaderFile = "shader/water/water.frag";
}

Engine::ProceduralWaterProgram::ProceduralWaterProgram(const Engine::ProceduralWaterProgram& other)
	:Engine::Program(other)
{
	tctrlShaderFile = other.tctrlShaderFile;
	tevalShaderFile = other.tevalShaderFile;

	uModelView = other.uModelView;
	uModelViewProj = other.uModelViewProj;
	uNormal = other.uNormal;

	uLightDepthMatrix = other.uLightDepthMatrix;
	uLightDepthMatrix1 = other.uLightDepthMatrix1;

	uDepthTexture = other.uDepthTexture;
	uDepthTexture1 = other.uDepthTexture1;

	uGBuffer = other.uGBuffer;
	uScreenSize = other.uScreenSize;

	uTime = other.uTime;
	uWaterColor = other.uWaterColor;
	uWaterSpeed = other.uWaterSpeed;

	uInPos = other.uInPos;
	uInUV = other.uInUV;
	uWorldGridPos = other.uWorldGridPos;
}

void Engine::ProceduralWaterProgram::Initialize()
{
	string configStr = "";

	if (paramters & Engine::ProceduralWaterProgram::WIRE_DRAW_MODE)
	{
		configStr += "#define WIRE_MODE";
	}
	else if (paramters & Engine::ProceduralWaterProgram::POINT_DRAW_MODE)
	{
		configStr += "#define POINT_MODE";
	}

	if (paramters & Engine::ProceduralWaterProgram::SHADOW_MAP)
	{
		configStr += "#define SHADOW_MAP";
	}

	if (paramters & Engine::ProceduralWaterProgram::WIRE_DRAW_MODE)
	{
		shaderPtr = new Shader(vShaderFile, gShaderFile, fShaderFile, configStr);
	}
	else
	{
		shaderPtr = new Shader(vShaderFile, fShaderFile);
	}

	ConfigureProgram();

}

void Engine::ProceduralWaterProgram::ConfigureProgram()
{
	uModelView = glGetUniformLocation(shaderPtr->GetShaderID(), "modelView");
	uModelViewProj = glGetUniformLocation(shaderPtr->GetShaderID(), "modelViewProj");
	uNormal = glGetUniformLocation(shaderPtr->GetShaderID(), "normal");
	uGridPos = glGetUniformLocation(shaderPtr->GetShaderID(), "gridPos");

	uLightDepthMatrix = glGetUniformLocation(shaderPtr->GetShaderID(), "lightDepthMat");
	uLightDepthMatrix1 = glGetUniformLocation(shaderPtr->GetShaderID(), "lightDepthMat1");
	uDepthTexture = glGetUniformLocation(shaderPtr->GetShaderID(), "depthTexture");
	uDepthTexture1 = glGetUniformLocation(shaderPtr->GetShaderID(), "depthTexture1");
	uLightDirection = glGetUniformLocation(shaderPtr->GetShaderID(), "lightDir");

	uInInfo = glGetUniformLocation(shaderPtr->GetShaderID(), "inInfo");
	uScreenSize = glGetUniformLocation(shaderPtr->GetShaderID(), "screenSize");

	uWaterColor = glGetUniformLocation(shaderPtr->GetShaderID(), "watercolor");
	uWaterSpeed = glGetUniformLocation(shaderPtr->GetShaderID(), "waterspeed");
	uTime = glGetUniformLocation(shaderPtr->GetShaderID(), "time");

	uInPos = glGetAttribLocation(shaderPtr->GetShaderID(), "inPos");
	uInUV = glGetAttribLocation(shaderPtr->GetShaderID(), "inUV");
}

void Engine::ProceduralWaterProgram::ConfigureMeshBuffer(Mesh* mesh)
{
	mesh->use();
	if (uInPos != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vboVertices);
		glVertexAttribPointer(uInPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(uInPos);
	}

	if (uInUV != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vboUVs);
		glVertexAttribPointer(uInUV, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(uInUV);
	}

}

void Engine::ProceduralWaterProgram::ApplyGlobalUniforms()
{
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Engine::CascadeShadowMaps::getInstance()->GetDepthTexture0()->GetTexture()->GetTextureID());
		glUniform1i(uDepthTexture, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, Engine::CascadeShadowMaps::getInstance()->GetDepthTexture1()->GetTexture()->GetTextureID());
		glUniform1i(uDepthTexture1, 1);

		glm::vec3 ld = glm::normalize(Engine::Setting::lightDirection);
		glUniform3fv(uLightDirection, 1, &ld[0]);

		glUniform1f(uTime, Engine::Time::timeSinceBegining);

		Engine::DeferredRenderer* dr = static_cast<Engine::DeferredRenderer*>(Engine::RenderManager::getInstance()->GetRenderer());
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, dr->GetGBufferInfo()->GetTexture()->GetTextureID());
		glUniform1i(uInInfo, 2);
		glUniform2f(uScreenSize, float(Engine::ScreenManager::SCREEN_WIDTH), float(Engine::ScreenManager::SCREEN_HEIGHT));

		glUniform1f(uWaterSpeed, Engine::Setting::waterSpeed);
		glUniform3fv(uWaterColor, 1, &Engine::Setting::waterColor[0]);
	}
}

void Engine::ProceduralWaterProgram::OnRenderObject(const Object* obj, Camera* cam)
{
	glm::mat4 modelView = cam->getViewMatrix() * obj->GetModelMatrix();
	glm::mat4 modelViewProj = cam->getProjectionMatrix() * modelView;
	glm::mat4 normal = glm::transpose(glm::inverse(modelView));

	glUniformMatrix4fv(uModelView, 1, GL_FALSE, &(modelView[0][0]));
	glUniformMatrix4fv(uModelViewProj, 1, GL_FALSE, &(modelViewProj[0][0]));
	glUniformMatrix4fv(uNormal, 1, GL_FALSE, &(normal[0][0]));
}

Engine::Program* Engine::ProceduralWaterProgramFactory::CreateProgram(ullong paramters)
{
	Engine::ProceduralWaterProgram* program = new Engine::ProceduralWaterProgram(Engine::ProceduralWaterProgram::PROGRAM_NAME, paramters);
	program->Initialize();
	return program;
}

void Engine::ProceduralWaterProgram::SetUniformGridPosition(unsigned int i, unsigned int j)
{
	glUniform2i(uGridPos, i, j);
}

void Engine::ProceduralWaterProgram::SetUniformLightDepthMatrix(const glm::mat4& ldm)
{
	glUniformMatrix4fv(uLightDepthMatrix, 1, GL_FALSE, &(ldm[0][0]));
}

void Engine::ProceduralWaterProgram::SetUniformLightDepthMatrix1(const glm::mat4& ldm)
{
	glUniformMatrix4fv(uLightDepthMatrix1, 1, GL_FALSE, &(ldm[0][0]));
}