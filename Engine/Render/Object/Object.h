#pragma once

#include <glm/glm.hpp>
#include <gl\glew.h>

#include "../../Util/common.h"
#include "../IRenderable.h"
#include "../Mesh/Mesh.h"
#include "../Texture/TextureInstance.h"


namespace Engine
{
	class Object : public IRenderable
	{
	public:
		string ALBEDO_TEX = "albedo";
		string NORMAL_TEX = "normal";
		string SPECULAR_TEX = "emissive";
		string EMISSIVE_TEX = "specular";

	private:
		Mesh* meshPtr;

		glm::mat4 modelMatrix;

		float angleR;
		glm::vec3 rotation;
		glm::vec3 translation;
		glm::vec3 scaleVector;

		GLenum renderMode;

		TextureInstance* albedo;
		TextureInstance* emissive;
		TextureInstance* normal;
		TextureInstance* specular;

		string shaderName;

	public:

		Object(Mesh* mi);
		~Object();

		const Mesh* GetMesh() const;

		void Rotate(float angle, glm::vec3 r);
		void Translate(glm::vec3 t);
		void Scale(glm::vec3 s);
		void SetTranslation(glm::vec3 t);
		void SetScale(glm::vec3 s);
		void SetModelMatrix(const glm::mat4& matrix);

		void SetShader(std::string shaderName);
		string GetShaderName();

		void SetRenderMode(GLenum renderMode);
		GLenum GetRenderMode();

		const glm::mat4& GetModelMatrix()const;

		void SetAlbedoTexture(TextureInstance* t);
		void SetNormalMapTexture(TextureInstance* t);
		void SetSpecularMapTexture(TextureInstance* t);
		void SetEmissiveTexture(TextureInstance* t);

		const TextureInstance* GetAlbedoTexture() const;
		const TextureInstance* GetNormalMapTexture() const;
		const TextureInstance* GetSpecularMapTexture() const;
		const TextureInstance* GetEmissiveTexture() const;

		virtual void NotifyRenderModeUpdate(RenderMode mode) override;

	private:
		void UpdateModelMatrix();
	};

	class PostProcessObject : public Object
	{
	private:
		std::map<string, TextureInstance*> buffers;
	public:
		PostProcessObject(Mesh* mi);

		void AddTexture(string name, Engine::TextureInstance* instance);
		TextureInstance* GetTextureByName(string name);
		const std::map<std::string, TextureInstance*>& GetAllCustomTextures() const;
	};
}


