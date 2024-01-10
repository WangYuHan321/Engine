#include "ProceduralTerrainProgram.h"
#include "../../Render/Shadow/CascadeShadowMaps.h"
#include "../../Config/WorldConfig.h"

Engine::string Engine::ProceduralTerrainProgram::PROGRAM_NAME = "ProceduralTerrainProgram";

const Engine::ullong Engine::ProceduralTerrainProgram::WIRE_DRAW_MODE = 0X01;
const Engine::ullong Engine::ProceduralTerrainProgram::POINT_DRAW_MODE = 0X02;
const Engine::ullong Engine::ProceduralTerrainProgram::SHADOW_MAP = 0X04;

Engine::ProceduralTerrainProgram::ProceduralTerrainProgram(string name, ullong paramters)
	:Engine::Program(name, paramters)
{
    vShaderFile = "shader/terrain/terrain.vert";
    tctrlShaderFile = "shader/terrain/terrain.tesctrl";
    tevalShaderFile = "shader/terrain/terrain.teseval";
    gShaderFile = "shader/terrain/terrain.geom";
    fShaderFile = "shader/terrain/terrain.frag";
}

Engine::ProceduralTerrainProgram::ProceduralTerrainProgram(const ProceduralTerrainProgram& other)
	:Engine::Program(other)
{
    uInPos = other.uInPos;
    uInUV = other.uInUV;

    uModelView = other.uModelView;
    uModelViewProj = other.uModelViewProj;
    uNormal = other.uNormal;

    uLightDepthMatrix = other.uLightDepthMatrix;
    uLightDepthMatrix1 = other.uLightDepthMatrix1;
    uDepthTexture = other.uDepthTexture;
    uDepthTexture1 = other.uDepthTexture1;

    uLightDirection = other.uLightDirection;

    uWaterLevel = other.uWaterLevel;
    uWorldScale = other.uWorldScale;
    uRenderRadius = other.uRenderRadius;

    uTime = other.uTime;

    uGridPos = other.uGridPos;
}

void Engine::ProceduralTerrainProgram::Initialize()
{
    std::string configStr = "";

    if (paramters & Engine::ProceduralTerrainProgram::WIRE_DRAW_MODE)
    {
        configStr += "#define WIRE_MODE";
    }
    else if (paramters & Engine::ProceduralTerrainProgram::POINT_DRAW_MODE)
    {
        configStr += "#define POINT_MODE";
    }

    if (paramters & Engine::ProceduralTerrainProgram::SHADOW_MAP)
    {
        configStr += "#define SHADOW_MAP";
    }

    shaderPtr = new Shader(vShaderFile, tctrlShaderFile, tevalShaderFile, gShaderFile, fShaderFile, configStr);
    
    ConfigureProgram();
}

void Engine::ProceduralTerrainProgram::ConfigureProgram()
{
    uModelView = glGetUniformLocation(shaderPtr->GetShaderID(), "modelView");
    uModelViewProj = glGetUniformLocation(shaderPtr->GetShaderID(), "modelViewProj");
    uNormal = glGetUniformLocation(shaderPtr->GetShaderID(), "normal");
    uGridPos = glGetUniformLocation(shaderPtr->GetShaderID(), "gridPos");

    uLightDepthMatrix = glGetUniformLocation(shaderPtr->GetShaderID(), "lightDepthMat");
    uLightDepthMatrix1 = glGetUniformLocation(shaderPtr->GetShaderID(), "lightDepthMat1");
    uLightDirection = glGetUniformLocation(shaderPtr->GetShaderID(), "lightDir");
    uDepthTexture = glGetUniformLocation(shaderPtr->GetShaderID(), "depthTexture");
    uDepthTexture1 = glGetUniformLocation(shaderPtr->GetShaderID(), "depthTexture1");

    uWaterLevel = glGetUniformLocation(shaderPtr->GetShaderID(), "waterHeight");
    uWorldScale = glGetUniformLocation(shaderPtr->GetShaderID(), "worldScale");
    uRenderRadius = glGetUniformLocation(shaderPtr->GetShaderID(), "renderRadius");

    uAmplitude = glGetUniformLocation(shaderPtr->GetShaderID(), "amplitude");
    uFrecuency = glGetUniformLocation(shaderPtr->GetShaderID(), "frecuency");
    uScale = glGetUniformLocation(shaderPtr->GetShaderID(), "scale");
    uOctaves = glGetUniformLocation(shaderPtr->GetShaderID(), "octaves");

    uGrassCoverage = glGetUniformLocation(shaderPtr->GetShaderID(), "grassCoverage");
    uGrassColor = glGetUniformLocation(shaderPtr->GetShaderID(), "grass");
    uRockColor = glGetUniformLocation(shaderPtr->GetShaderID(), "rock");
    uSandColor = glGetUniformLocation(shaderPtr->GetShaderID(), "sand");
    uTime = glGetUniformLocation(shaderPtr->GetShaderID(), "time");

    uInPos = glGetAttribLocation(shaderPtr->GetShaderID(), "inPos");
    uInUV = glGetAttribLocation(shaderPtr->GetShaderID(), "inUV");
}

void Engine::ProceduralTerrainProgram::ConfigureMeshBuffer(Mesh* mesh)
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

void Engine::ProceduralTerrainProgram::ApplyGlobalUniforms()
{
    if (!(paramters & Engine::ProceduralTerrainProgram::SHADOW_MAP))
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Engine::CascadeShadowMaps::getInstance()->GetDepthTexture0()->GetTexture()->GetTextureID());
        glUniform1i(uDepthTexture, 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, Engine::CascadeShadowMaps::getInstance()->GetDepthTexture1()->GetTexture()->GetTextureID());
        glUniform1i(uDepthTexture1, 1);

        glm::vec3 ld = glm::normalize(Engine::Setting::lightDirection);
        glUniform3fv(uLightDirection, 1, &ld[0]);

        glUniform1f(uGrassCoverage, 1.0f - Engine::Setting::grassCoverage);
        glUniform3fv(uGrassColor, 1, &Engine::Setting::grassColor[0]);
        glUniform3fv(uRockColor, 1, &Engine::Setting::rockColor[0]);
        glUniform3fv(uSandColor, 1, &Engine::Setting::sandColor[0]);

        glUniform1f(uTime, 0.05F);

        glUniform1f(uWorldScale, Engine::Setting::worldTileScale);
        glUniform1f(uRenderRadius, (float)Engine::Setting::worldRenderRadius);
    }

    glUniform1f(uWorldScale, Engine::Setting::worldTileScale);
    glUniform1f(uAmplitude, Engine::Setting::terrainAmplitude);
    glUniform1f(uFrecuency, Engine::Setting::terrainFrecuency);
    glUniform1f(uScale, Engine::Setting::terrainScale);
    glUniform1i(uOctaves, Engine::Setting::terrainOctaves);
    glUniform1f(uWaterLevel, Engine::Setting::waterHeight);
}

void Engine::ProceduralTerrainProgram::OnRenderObject(const Object* obj, Camera* cam)
{
    glm::mat4 modelView = cam->getViewMatrix() * obj->GetModelMatrix();
    glm::mat4 modelViewProj = cam->getProjectionMatrix() * modelView;
    glm::mat4 normal = glm::transpose(glm::inverse(modelView));

    glUniformMatrix4fv(uModelView, 1, GL_FALSE, &(modelView[0][0]));
    glUniformMatrix4fv(uModelViewProj, 1, GL_FALSE, &(modelViewProj[0][0]));
    glUniformMatrix4fv(uNormal, 1, GL_FALSE, &(normal[0][0]));

    //patch 最少个数是3
    unsigned int vertexPerFace = obj->GetMesh()->getNumVerticesPerFace();
    glPatchParameteri(GL_PATCH_VERTICES, vertexPerFace);
}

void Engine::ProceduralTerrainProgram::Destroy()
{
    
}

void Engine::ProceduralTerrainProgram::SetUniformGridPosition(uint32 i, uint32 j)
{
    glUniform2i(uGridPos, i, j);
}

void Engine::ProceduralTerrainProgram::SetUniformLightDepthMatrix(const glm::mat4& ldm)
{
    glUniformMatrix4fv(uLightDepthMatrix, 1, GL_FALSE, &(ldm[0][0]));
}

void Engine::ProceduralTerrainProgram::SetUniformLightDepthMatrix1(const glm::mat4& ldm)
{
    glUniformMatrix4fv(uLightDepthMatrix1, 1, GL_FALSE, &(ldm[0][0]));
}

Engine::Program* Engine::ProceduralTerrainProgramFactory::CreateProgram(ullong paramter)
{
    Engine::ProceduralTerrainProgram* program = new Engine::ProceduralTerrainProgram(Engine::ProceduralTerrainProgram::PROGRAM_NAME, paramter);
    program->Initialize();
    return program;
}