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
		//ת����ͼ����ת�����ߺ͹��ߣ�
		uint32 uInvView;

		//Perlin-Worley��ά�������� ID�������Ƶ���װ��
		uint32 uPerlinWorley;
		//Worley 3D noise texture ID (�Ƶ�ϸ��)
		uint32 uWorley;
		//����������Ϣ ID 			
		//r = �Ƹ��ǣ�Clamped Perlin����� perling < 0.7��Ϊ 0������Ϊ perlin��
		//g = ��
		//b = ������
		uint32 uWeather;

		//��Ļ�ֱ���
		uint32 uResolution;
		//�����FOV
		uint32 uFOV;
		uint32 uProjView;

		//��������Ʋʲ���
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

		//�������
		uint32 uCamPos;
		//ƽ�йⷽ��
		uint32 uLightDir;
		//�����ɫ
		uint32 uLightColor;
		//�������� (�����ڹ��λ��)
		uint32 uLightFactor;
		// �춥��ɫ  (fog / ambient)
		uint32 uZenitColor;
		// ˮƽ����ɫ (fog / ambient)
		uint32 uHorizonColor;
		// �Ʋ���ɫ
		uint32 uCloudColor;
		// ʱ��
		uint32 uTime;
		// �Ƶ�����
		uint32 uCloudType;
		// �Ƶ��ٶ�
		uint32 uCloudSpeed;
		// ��ķ���
		uint32 uWindDirection;
		// ���Ƿ�Χ���ƣ�
		uint32 uCoverageMultiplier;

		// �������
		uint32 uCurrentDepth;

		// ��ǰ֡ id
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


