#pragma once

#include "../Texture/TextureInstance.h"
#include "../Object/Object.h"

namespace Engine
{

	struct BufferInfo
	{
		GLenum bufferType; //GL_DEPTH_ATTACHMENT
		TextureInstance* texture;
	} typedef BufferInfo;

	class DeferredRenderObject
	{
	private:
		static GLenum COLOR_ATTACHMENTS[8];

	public:

		// Default texture names
		static const std::string G_BUFFER_COLOR;
		static const std::string G_BUFFER_POS;
		static const std::string G_BUFFER_NORMAL;
		static const std::string G_BUFFER_SPECULAR;
		static const std::string G_BUFFER_DEPTH;
		static const std::string G_BUFFER_EMISSIVE;

	private:
		uint32 fbo;

		int usedColorBuffers;
		uint32 colorBuffersSize;

		BufferInfo* colorBuffers; //使用普通类型的指针需要delete
		BufferInfo depthBuffer;
		std::map<string, TextureInstance*>gBufferMap;

		bool renderDepth;
		float widthMod, heightMod;

	public:
		DeferredRenderObject(uint32 numColorBuffers, bool renderDepth);
		~DeferredRenderObject();

		uint32 GetFrameBufferID() const;

		TextureInstance* AddColorBuffer(uint32 index, GLenum gpuTextureFormat, GLenum inputTextureFormat, GLenum pixelFormat, uint32 w, uint32 h, string name, int filterMethod);
		TextureInstance* AddDepthBuffer24(unsigned int w, unsigned int h);
		TextureInstance* AddDepthBuffer32(unsigned int w, unsigned int h);
		TextureInstance* GetBufferByName(std::string name);

		void Initialize();
		void SetReSizeMod(float wm = 1.0f, float hm = 1.0f);
		void ReSizeFBO(uint32 w, uint32 h);


		void PopulateDeferredObject(PostProcessObject* obj);
	};

}


