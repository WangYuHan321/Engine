#pragma once
#include "Program.h"

namespace Engine
{
    class ProceduralTerrainProgram :
        public Program
    {
    public:
        static string PROGRAM_NAME;

        static const ullong WIRE_DRAW_MODE;
        static const ullong POINT_DRAW_MODE;
        static const ullong SHADOW_MAP;

    protected:

        string tctrlShaderFile;
        string tevalShaderFile;

        uint32 uInPos;
        uint32 uInUV;

        uint32 uModelView;
        uint32 uModelViewProj;
        uint32 uNormal;
        
        uint32 uLightDepthMatrix;
        uint32 uLightDepthMatrix1;
        uint32 uDepthTexture;
        uint32 uDepthTexture1;

        uint32 uLightDirection;

        uint32 uWaterLevel;
        uint32 uWorldScale;
        uint32 uRenderRadius;

        uint32 uTime;

        uint32 uGridPos;

        unsigned int uAmplitude;
        unsigned int uFrecuency;
        unsigned int uScale;
        unsigned int uOctaves;


        unsigned int uGrassCoverage;
        unsigned int uGrassColor;
        unsigned int uSandColor;
        unsigned int uRockColor;

    public:
        ProceduralTerrainProgram(string name, ullong paramters);
        ProceduralTerrainProgram(const ProceduralTerrainProgram& other);

        virtual void Initialize();
        virtual void ConfigureProgram() override;
        virtual void ConfigureMeshBuffer(Mesh* mesh);

        virtual void ApplyGlobalUniforms();
        virtual void OnRenderObject(const Object* obj, Camera* cam);

        virtual void Destroy();

        void SetUniformGridPosition(uint32 i, uint32 j);
        void SetUniformLightDepthMatrix(const glm::mat4& ldm);
        void SetUniformLightDepthMatrix1(const glm::mat4& ldm);

    };

    class ProceduralTerrainProgramFactory : public ProgramFactory
    {
    public:
        virtual Program* CreateProgram(ullong paramter);
    };
}


