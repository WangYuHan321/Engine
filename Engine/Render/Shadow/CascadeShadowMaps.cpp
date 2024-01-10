#include "CascadeShadowMaps.h"

#include "../../Scene/Scene.h"
#include <glm/gtc/matrix_transform.hpp>

void Engine::CascadeShadowMaps::Init()
{
	InitializeShadowMap(0, -5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 70.0f);
	InitializeShadowMap(1, -40.0f, 40.0f, -40.0f, 40.0f, 0.1f, 120.0f);
	//InitializeShadowMap(2, -5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 680.0f);
}

void Engine::CascadeShadowMaps::InitializeFrame(Engine::Camera* eye)
{
	Engine::DirectionalLight* dl = Engine::SceneManager::getInstance()->GetActiveScene()->GetDirectionLight();
	const glm::vec3& cameraPosition = eye->getPosition();
	glm::vec3 target = glm::vec3(-cameraPosition.x, 0, -cameraPosition.z);
	glm::mat4 depthViewMatrix = glm::lookAt(target + (dl->GetDirection() * 50.0f), target, glm::vec3(0, 1, 0));

	for (unsigned int i = 0; i < GetCascadeLevel(); i++)
	{
		shadowMap[i].depth = shadowMap[i].proj * depthViewMatrix;
	}

	RenderShadows(eye);
	
}

void Engine::CascadeShadowMaps::BeginShadowRender(int level)
{
	currentLevel = level;
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMap[currentLevel].rtt->GetFrameBufferID());
	glClear(GL_DEPTH_BUFFER_BIT);
}

void Engine::CascadeShadowMaps::EndShadowRender()
{
	shadowMap[currentLevel].depth = biasMatrix * shadowMap[currentLevel].depth;
}

void Engine::CascadeShadowMaps::RenderShadows(Engine::Camera* cam)
{
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFrameBuffer);

	for (unsigned int i = 0; i < GetCascadeLevel(); i++)
	{
		BeginShadowRender(i);

		for (auto& v : shadowCasters)
		{
			v->RenderShadow(cam, GetShadowProjectionMat());
		}

		EndShadowRender();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, previousFrameBuffer);
}

void Engine::CascadeShadowMaps::InitializeShadowMap(int level, float minX, float maxX, float miny, float maxY, float minz, float maxZ)
{
	shadowMap[level].proj = glm::ortho<float>(minX, maxX, miny, maxY, minz, maxZ);
	shadowMap[level].rtt = new Engine::DeferredRenderObject(0, true);
	shadowMap[level].shadowMap = shadowMap[level].rtt->AddDepthBuffer24(1024, 1024);
	shadowMap[level].rtt->Initialize();
}

void Engine::CascadeShadowMaps::RegisterShadowCaster(ShadowCaster* caster)
{
	shadowCasters.push_back(caster);
}


