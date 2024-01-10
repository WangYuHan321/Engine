#pragma once

#include <glm\glm.hpp>
#include "../../Util/common.h"

namespace Engine
{
	class Light
	{
	private:
		bool enabled;
		string name;

		uint32 bufferIndex;
	protected:
		glm::mat4 modelMatrix;
		bool update;

	public:
		Light(string name);
		~Light();

		const string& GetName()const;

		void SetEnabled(bool val);
		const bool IsEnabled() const;

		virtual void SetColor(const glm::vec3& color) = 0;
		virtual void SetKa(float a) = 0;
		virtual void SetKd(float d) = 0;
		virtual void SetKs(float s) = 0;

		virtual void Translate(const glm::vec3& translation) = 0;

		const glm::mat4& GetModelMatrix() const;

		// Returns wether the uniform buffer object should be updated or not
		bool RequiresUpdate();
		// Clears the udpate flag
		void ClearUpdateFlag();

		void SetBufferIndex(unsigned int bi);
		unsigned int GetBufferIndex();

	};
}



