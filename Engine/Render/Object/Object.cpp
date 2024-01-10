#include "Object.h"

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "../Notifier/RenderableNotifier.h"
#include "../../Config/WorldConfig.h"

Engine::Object::Object(Engine::Mesh* m)
	:meshPtr(m)
{
	modelMatrix = glm::mat4(1.0f);
	translation = glm::vec3(0, 0, 0);
	rotation = glm::vec3(1, 1, 1);
	scaleVector = glm::vec3(1, 1, 1);
	angleR = 0.0f;

	albedo = normal = specular = emissive = 0;

	renderMode = GL_TRIANGLES;

	Engine::RenderableNotifier::getInstance()->RegisterRenderable(this);
}

Engine::Object::~Object()
{

}

const glm::mat4& Engine::Object::GetModelMatrix() const
{
	return modelMatrix;
}

const Engine::Mesh* Engine::Object::GetMesh() const
{
	return meshPtr;
}

void Engine::Object::Rotate(float angle, glm::vec3 r)
{
	rotation += r;
	angleR = angle;

	UpdateModelMatrix();
}

void Engine::Object::Translate(glm::vec3 t)
{
	translation += t;

	UpdateModelMatrix();
}

void Engine::Object::Scale(glm::vec3 s)
{
	scaleVector += s;
	UpdateModelMatrix();
}

void Engine::Object::SetTranslation(glm::vec3 t)
{
	translation = t;
	UpdateModelMatrix();
}

void Engine::Object::SetScale(glm::vec3 s)
{
	scaleVector = s;
	UpdateModelMatrix();
}

void Engine::Object::SetModelMatrix(const glm::mat4& matrix)
{
	modelMatrix = glm::mat4(matrix);
}

void Engine::Object::UpdateModelMatrix()
{
	modelMatrix = glm::mat4(1.0f);

	glm::mat4 transMat = glm::translate(glm::mat4(1.0f), translation);
	glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), angleR, rotation);
	glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), scaleVector);

	modelMatrix = transMat * rotMat * scaleMat;
}

void Engine::Object::SetRenderMode(GLenum renderMode)
{
	this->renderMode = renderMode;
}

GLenum Engine::Object::GetRenderMode()
{
	return renderMode;
}

void Engine::Object::SetAlbedoTexture(Engine::TextureInstance* t)
{
	if (albedo != 0)
	{
		albedo->~TextureInstance();
		delete albedo;
	}

	albedo = t;
}

void Engine::Object::SetEmissiveTexture(Engine::TextureInstance* t)
{
	if (emissive != 0)
	{
		delete emissive;
	}

	emissive = t;
}

void Engine::Object::SetNormalMapTexture(Engine::TextureInstance* t)
{
	if (normal != 0)
	{
		delete normal;
	}

	normal = t;
}

void Engine::Object::SetSpecularMapTexture(TextureInstance* t)
{
	if (specular != 0)
	{
		delete specular;
	}

	specular = t;
}

const Engine::TextureInstance* Engine::Object::GetAlbedoTexture() const
{
	return albedo;
}

const Engine::TextureInstance* Engine::Object::GetEmissiveTexture() const
{
	return emissive;
}

const Engine::TextureInstance* Engine::Object::GetNormalMapTexture() const
{
	return normal;
}

const Engine::TextureInstance* Engine::Object::GetSpecularMapTexture() const
{
	return specular;
}

void Engine::Object::SetShader(std::string shaderName)
{
	this->shaderName = shaderName;
}

std::string Engine::Object::GetShaderName()
{
	return this->shaderName;
}



void Engine::Object::NotifyRenderModeUpdate(Engine::RenderMode mode)
{
	switch (mode)
	{
	case Engine::RenderMode::RENDER_MODE_WIRE:
		SetRenderMode(GL_LINES);
		break;
	default:
		SetRenderMode(GL_TRIANGLES);
		break;
	}
}

Engine::PostProcessObject::PostProcessObject(Mesh* mi):
	Engine::Object(mi)
{
}

void Engine::PostProcessObject::AddTexture(string name, Engine::TextureInstance* instance)
{
	buffers[name] = instance;
}

Engine::TextureInstance* Engine::PostProcessObject::GetTextureByName(string name)
{
	std::map<std::string, Engine::TextureInstance*>::iterator it = buffers.find(name);
	if (it == buffers.end())
	{
		return nullptr;
	}

	return buffers[name];
}

const std::map<std::string, Engine::TextureInstance*>& Engine::PostProcessObject::GetAllCustomTextures() const
{
	return buffers;
}