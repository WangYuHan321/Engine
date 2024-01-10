#pragma once
#include "../PostProcessProgram/PostProcessProgram.h"

namespace Engine
{
	class VolumetricCloudProgram :
		public PostProcessProgram
	{
	public:
		static const string PROGRAM_NAME;

	private:
		//转置视图矩阵（转换射线和光线）
		uint32 uInvView;

		//Perlin-Worley三维噪声纹理 ID（绘制云的形装）
		uint32 uPerlinWorley;
		//Worley 3D noise texture ID (云的细节)
		uint32 uWorley;
		//天气纹理信息 ID 			
		//r = 云覆盖（Clamped Perlin：如果 perling < 0.7，为 0，否则为 perlin）
		//g = 雨
		//b = 云类型
		uint32 uWeather;

		//屏幕分辨率
		uint32 uResolution;
		//相机的FOV
		uint32 uFOV;
		uint32 uProjView;

		//绘制天空云彩参数
		uint32 uSphereCenter;
		uint32 uInnerSphereRadius;
		uint32 uOuterSphereRadius;
		uint32 uMaxDrawDistance;
		uint32 uTopOffset;
		uint32 uWeatherScale;
		uint32 uBaseNoiseScale;
		uint32 uHighFreqNoiseScale;
		uint32 uHighFreqNoiseUVScale;
		uint32 uHighFreqNoiseHScale;

		//相机坐标
		uint32 uCamPos;
		//平行光方向
		uint32 uLightDir;
		//光的颜色
		uint32 uLightColor;
		//光照因子 (依赖于光的位置)
		uint32 uLightFactor;
		// 天顶颜色  (fog / ambient)
		uint32 uZenitColor;
		// 水平线颜色 (fog / ambient)
		uint32 uHorizonColor;
		// 云彩颜色
		uint32 uCloudColor;
		// 时间
		uint32 uTime;
		// 云的类型
		uint32 uCloudType;
		// 云的速度
		uint32 uCloudSpeed;
		// 风的方向
		uint32 uWindDirection;
		// 覆盖范围（云）
		uint32 uCoverageMultiplier;

		// 深度纹理
		uint32 uCurrentDepth;

		// 当前帧 id
		uint32 uFrame;

	public:
		VolumetricCloudProgram(string name, ullong parameter);
		VolumetricCloudProgram(const VolumetricCloudProgram& other);

		void ConfigureProgram();
		void OnRenderObject(Object* obj, Camera* camera);

	};

	class VolumetricCloudProgramFactory : public ProgramFactory
	{
	public:
		Program* CreateProgram(unsigned long long parameters);
	};

}


