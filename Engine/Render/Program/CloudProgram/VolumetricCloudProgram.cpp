#include "VolumetricCloudProgram.h"
#include "../../Scene/Scene.h"
#include"../../Config/WorldConfig.h"
#include "../../../Util/Time.h"
#include"../../Render/DeferredRender/DeferredRenderer.h"
#include "../../Render/Init/NoiseInitializer.h"

const Engine::string Engine::VolumetricCloudProgram::PROGRAM_NAME = "VolumetricCloudProgram";

Engine::VolumetricCloudProgram::VolumetricCloudProgram(string name, ullong parameter) :
	Engine::PostProcessProgram(name, parameter)
{
	fShaderFile = "shader/clouds/volumetricclouds.frag";
}

Engine::VolumetricCloudProgram::VolumetricCloudProgram(const VolumetricCloudProgram& other) : 
	Engine::PostProcessProgram(other)
{
	//ת����ͼ����ת�����ߺ͹��ߣ�
	uInvView = other.uInvView;

	//Perlin-Worley��ά�������� ID�������Ƶ���װ��
	uPerlinWorley = other.uPerlinWorley;
	//Worley 3D noise texture ID (�Ƶ�ϸ��)
	uWorley = other.uWorley;
	//����������Ϣ ID 			
	//r = �Ƹ��ǣ�Clamped Perlin����� perling < 0.7��Ϊ 0������Ϊ perlin��
	//g = ��
	//b = ������
	uWeather = other.uWeather;

	//��Ļ�ֱ���
	uResolution = other.uResolution;
	//�����FOV
	uFOV = other.uFOV;
	uProjView = other.uProjView;

	//��������Ʋʲ���
	uSphereCenter = other.uSphereCenter;
	uInnerSphereRadius = other.uInnerSphereRadius;
	uOuterSphereRadius = other.uOuterSphereRadius;
	uMaxDrawDistance = other.uMaxDrawDistance;
	uTopOffset = other.uTopOffset;
	uWeatherScale = other.uWeatherScale;
	uBaseNoiseScale = other.uBaseNoiseScale;
	uHighFreqNoiseScale = other.uHighFreqNoiseScale;
	uHighFreqNoiseUVScale = other.uHighFreqNoiseUVScale;
	uHighFreqNoiseHScale = other.uHighFreqNoiseHScale;

	//�������
	uCamPos = other.uCamPos;
	//ƽ�йⷽ��
	uLightDir = other.uLightDir;
	//�����ɫ
	uLightColor = other.uLightColor;
	//�������� (�����ڹ��λ��)
	uLightFactor = other.uLightFactor;
	// �춥��ɫ  (fog / ambient)
	uZenitColor = other.uZenitColor;
	// ˮƽ����ɫ (fog / ambient)
	uHorizonColor = other.uHorizonColor;
	// �Ʋ���ɫ
	uCloudColor = other.uCloudColor;
	// ʱ��
	uTime = other.uTime;
	// �Ƶ�����
	uCloudType = other.uCloudType;
	// �Ƶ��ٶ�
	uCloudSpeed = other.uCloudSpeed;
	// ��ķ���
	uWindDirection = other.uWindDirection;
	// ���Ƿ�Χ���ƣ�
	uCoverageMultiplier = other.uCoverageMultiplier;

	// �������
	uCurrentDepth = other.uCurrentDepth;

	// ��ǰ֡ id
	uFrame = other.uFrame;
}

void Engine::VolumetricCloudProgram::ConfigureProgram()
{
	Engine::PostProcessProgram::ConfigureProgram();

	uInvView = glGetUniformLocation(shaderPtr->GetShaderID(), "invView");

	uResolution = glGetUniformLocation(shaderPtr->GetShaderID(), "screenResolution");
	uFOV = glGetUniformLocation(shaderPtr->GetShaderID(), "FOV");

	uPerlinWorley = glGetUniformLocation(shaderPtr->GetShaderID(), "perlinworley");
	uWorley = glGetUniformLocation(shaderPtr->GetShaderID(), "worley");
	uWeather = glGetUniformLocation(shaderPtr->GetShaderID(), "weather");

	uProjView = glGetUniformLocation(shaderPtr->GetShaderID(), "projView");

	uCamPos = glGetUniformLocation(shaderPtr->GetShaderID(), "camPos");
	uSphereCenter = glGetUniformLocation(shaderPtr->GetShaderID(), "sphereCenter");
	uInnerSphereRadius = glGetUniformLocation(shaderPtr->GetShaderID(), "innerSphereRadius");
	uOuterSphereRadius = glGetUniformLocation(shaderPtr->GetShaderID(), "outerSphereRadius");
	uLightDir = glGetUniformLocation(shaderPtr->GetShaderID(), "lightDir");
	uLightColor = glGetUniformLocation(shaderPtr->GetShaderID(), "realLightColor");
	uLightFactor = glGetUniformLocation(shaderPtr->GetShaderID(), "lightFactor");
	uCloudColor = glGetUniformLocation(shaderPtr->GetShaderID(), "cloudColor");
	uZenitColor = glGetUniformLocation(shaderPtr->GetShaderID(), "zenitColor");
	uHorizonColor = glGetUniformLocation(shaderPtr->GetShaderID(), "horizonColor");
	uTime = glGetUniformLocation(shaderPtr->GetShaderID(), "time");

	uCloudSpeed = glGetUniformLocation(shaderPtr->GetShaderID(), "cloudSpeed");
	uWindDirection = glGetUniformLocation(shaderPtr->GetShaderID(), "windDirection");
	uCloudType = glGetUniformLocation(shaderPtr->GetShaderID(), "cloudType");
	uCoverageMultiplier = glGetUniformLocation(shaderPtr->GetShaderID(), "coverageMultiplier");

	uTopOffset = glGetUniformLocation(shaderPtr->GetShaderID(), "cloudTopOffset");

	uWeatherScale = glGetUniformLocation(shaderPtr->GetShaderID(), "weatherScale");
	uBaseNoiseScale = glGetUniformLocation(shaderPtr->GetShaderID(), "baseNoiseScale");
	uHighFreqNoiseScale = glGetUniformLocation(shaderPtr->GetShaderID(), "highFreqNoiseScale");
	uHighFreqNoiseUVScale = glGetUniformLocation(shaderPtr->GetShaderID(), "highFreqNoiseUVScale");
	uHighFreqNoiseHScale = glGetUniformLocation(shaderPtr->GetShaderID(), "highFreqNoiseHScale");

	uCurrentDepth = glGetUniformLocation(shaderPtr->GetShaderID(), "currentPixelDepth");

	uFrame = glGetUniformLocation(shaderPtr->GetShaderID(), "frame");
	uMaxDrawDistance = glGetUniformLocation(shaderPtr->GetShaderID(), "maxRenderDist");
}

void Engine::VolumetricCloudProgram::OnRenderObject(Object* obj, Camera* camera)
{
	glm::mat4 invView = glm::inverse(camera->getViewMatrix());
	glUniformMatrix4fv(uInvView, 1, GL_FALSE, &(invView[0][0]));

	glm::mat4 pV = camera->getProjectionMatrix() * camera->getViewMatrix();
	glUniformMatrix4fv(uProjView, 1, GL_FALSE, &(pV[0][0]));

	Engine::Scene* scene = Engine::SceneManager::getInstance()->GetActiveScene();
	Engine::Camera* cam = scene->GetCamera();
	glm::vec3 camPos = -cam->getPosition();
	glUniform3fv(uCamPos, 1, &camPos[0]);

	glm::vec4 spherePos = glm::vec4(camPos[0], Setting::sphereYOffset, camPos[2], 1.0f);
	glUniform3fv(uSphereCenter, 1, &spherePos[0]);

	glUniform1f(uInnerSphereRadius, Setting::innerSphereRadius);
	glUniform1f(uOuterSphereRadius, Setting::outerSphereRadius);
	glUniform1f(uMaxDrawDistance, Setting::cloudMaxRenderDistance);
	glUniform1f(uTopOffset, Setting::cloudTopOffset);
	glUniform1f(uWeatherScale, Setting::weatherTextureScale);
	glUniform1f(uBaseNoiseScale, Setting::baseNoiseScale);
	glUniform1f(uHighFreqNoiseScale, Setting::highFrequencyNoiseScale);
	glUniform1f(uHighFreqNoiseUVScale, Setting::highFrequencyNoiseUVScale);
	glUniform1f(uHighFreqNoiseHScale, Setting::highFrequencyNoiseHScale);

	glm::vec3 normLightDir = glm::normalize(Engine::Setting::lightDirection);
	glUniform3fv(uLightDir, 1, &normLightDir[0]);
	glUniform3fv(uLightColor, 1, &Engine::Setting::realLightColor[0]);
	glUniform1f(uLightFactor, Engine::Setting::lightFactor);

	glUniform3fv(uZenitColor, 1, &Engine::Setting::skyZenitColor[0]);
	glUniform3fv(uHorizonColor, 1, &Engine::Setting::skyHorizonColor[0]);

	glUniform3fv(uCloudColor, 1, &Engine::Setting::cloudColor[0]);
	glUniform1f(uTime, Engine::Time::timeSinceBegining);
	glUniform1f(uCloudSpeed, Engine::Setting::windStrength);
	glUniform3fv(uWindDirection, 1, &Engine::Setting::windDirection[0]);
	glUniform1f(uCloudType, Engine::Setting::cloudType);
	glUniform1f(uCoverageMultiplier, Engine::Setting::coverageMultiplier);

	float res[2] = { (float)Engine::ScreenManager::SCREEN_WIDTH, (float)Engine::ScreenManager::SCREEN_HEIGHT };
	glUniform2fv(uResolution, 1, res);
	glUniform1f(uFOV, cam->getFOV());

	const Engine::TextureInstance* pw = Engine::CloudSystem::NoiseInitializer::getInstance()->GetPerlinWorleyFBM();
	const Engine::TextureInstance* w = Engine::CloudSystem::NoiseInitializer::getInstance()->GetWorleyFBM();
	const Engine::TextureInstance* wth = Engine::CloudSystem::NoiseInitializer::getInstance()->GetWeatherData();

	glUniform1i(uPerlinWorley, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, pw->GetTexture()->GetTextureID());

	glUniform1i(uWorley, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_3D, w->GetTexture()->GetTextureID());

	glUniform1i(uWeather, 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, wth->GetTexture()->GetTextureID());

	Engine::DeferredRenderer* dr = static_cast<Engine::DeferredRenderer*>(Engine::RenderManager::getInstance()->GetRenderer());
	glUniform1i(uCurrentDepth, 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, dr->GetGBufferDepth()->GetTexture()->GetTextureID());

	glUniform1i(uFrame, (GLint)Engine::Time::frame);
}

Engine::Program* Engine::VolumetricCloudProgramFactory::CreateProgram(unsigned long long parameters)
{
	Engine::VolumetricCloudProgram* prog = new Engine::VolumetricCloudProgram(Engine::VolumetricCloudProgram::PROGRAM_NAME, parameters);
	prog->Initialize();
	return prog;
}