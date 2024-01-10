#include "DeferredRenderObject.h"
#include "../../Table/DeferredRenderObjectsTable.h"
#include "../Texture/Texture2D.h"

const std::string Engine::DeferredRenderObject::G_BUFFER_COLOR = "g_buffer_color";
const std::string Engine::DeferredRenderObject::G_BUFFER_DEPTH = "g_buffer_depth";
const std::string Engine::DeferredRenderObject::G_BUFFER_NORMAL = "g_buffer_normal";
const std::string Engine::DeferredRenderObject::G_BUFFER_POS = "g_buffer_pos";
const std::string Engine::DeferredRenderObject::G_BUFFER_SPECULAR = "g_buffer_depth";
const std::string Engine::DeferredRenderObject::G_BUFFER_EMISSIVE = "g_buffer_emissive";

GLenum Engine::DeferredRenderObject::COLOR_ATTACHMENTS[8] =
{
	GL_COLOR_ATTACHMENT0,
	GL_COLOR_ATTACHMENT1,
	GL_COLOR_ATTACHMENT2,
	GL_COLOR_ATTACHMENT3,
	GL_COLOR_ATTACHMENT4,
	GL_COLOR_ATTACHMENT5,
	GL_COLOR_ATTACHMENT6,
	GL_COLOR_ATTACHMENT7
};

Engine::DeferredRenderObject::DeferredRenderObject(uint32 numBuffers, bool renderDepth)
	:
	colorBuffersSize(numBuffers),
	renderDepth(renderDepth),
	colorBuffers(0)
{
	if (numBuffers > 0)
	{
		colorBuffers = new Engine::BufferInfo[numBuffers];
	}
	usedColorBuffers = 0;
	depthBuffer.texture = 0;
	widthMod = heightMod = 1.0;

	DeferredRenderObjectsTable::getInstance()->RegisterDeferredObject(this);
}

Engine::DeferredRenderObject::~DeferredRenderObject()
{
	if (colorBuffers != 0)
	{
		for (unsigned int i = 0; i < colorBuffersSize; i++)
		{
			BufferInfo bi = colorBuffers[i];
			if (bi.texture != NULL)
			{
				if (bi.texture->GetTexture() != NULL)
				{
					delete bi.texture->GetTexture();
				}
				delete bi.texture;
			}
		}

		delete[] colorBuffers;
	}

	if (depthBuffer.texture != NULL)
	{
		if (depthBuffer.texture->GetTexture() != NULL)
		{
			delete depthBuffer.texture->GetTexture();
		}
		delete depthBuffer.texture;
	}
}

Engine::uint32 Engine::DeferredRenderObject::GetFrameBufferID() const
{
	return fbo;
}

Engine::TextureInstance* Engine::DeferredRenderObject::AddColorBuffer(uint32 index, GLenum gpuTextureFormat, GLenum inputTextureFormat, GLenum pixelFormat, uint32 w, uint32 h, std::string name, int filterMethod)
{
	if (index < 0 || index > colorBuffersSize || usedColorBuffers >= 8)
		exit(-1);

	GLenum colorAttachment = COLOR_ATTACHMENTS[usedColorBuffers];
	usedColorBuffers++;

	Texture2D* texture = new Engine::Texture2D(name, 0, w, h);
	texture->SetGenerateMipMaps(false);
	texture->SetMemoryLayoutFormat(gpuTextureFormat);
	texture->SetImageFormatType(inputTextureFormat);
	texture->SetPixelFormatType(pixelFormat);

	Engine::TextureInstance* ti = new Engine::TextureInstance(texture);
	ti->SetMagnificationFilterType(filterMethod);
	ti->SetMinificationFilterType(filterMethod);
	ti->SetSComponentWrapType(GL_CLAMP_TO_EDGE);
	ti->SetTComponentWrapType(GL_CLAMP_TO_EDGE);

	colorBuffers[index].bufferType = colorAttachment;
	colorBuffers[index].texture = ti;

	gBufferMap[name] = ti;

	return ti;
}

Engine::TextureInstance* Engine::DeferredRenderObject::AddDepthBuffer24(unsigned int w, unsigned int h)
{
	if (depthBuffer.texture != 0)
	{
		delete depthBuffer.texture;
	}

	Engine::Texture2D* texture = new Engine::Texture2D(Engine::DeferredRenderObject::G_BUFFER_DEPTH, 0, w, h);
	texture->SetGenerateMipMaps(false);
	texture->SetMemoryLayoutFormat(GL_DEPTH_COMPONENT24);
	texture->SetImageFormatType(GL_DEPTH_COMPONENT);
	texture->SetPixelFormatType(GL_FLOAT);

	Engine::TextureInstance* textureInstance = new Engine::TextureInstance(texture);
	textureInstance->SetMagnificationFilterType(GL_NEAREST);
	textureInstance->SetMinificationFilterType(GL_NEAREST);
	textureInstance->SetSComponentWrapType(GL_CLAMP_TO_EDGE);
	textureInstance->SetTComponentWrapType(GL_CLAMP_TO_EDGE);

	depthBuffer.bufferType = GL_DEPTH_ATTACHMENT;
	depthBuffer.texture = textureInstance;

	gBufferMap[Engine::DeferredRenderObject::G_BUFFER_DEPTH] = textureInstance;

	return textureInstance;
}

Engine::TextureInstance* Engine::DeferredRenderObject::AddDepthBuffer32(unsigned int w, unsigned int h)
{
	if (depthBuffer.texture != 0)
	{
		delete depthBuffer.texture;
	}

	Engine::Texture2D* texture = new Engine::Texture2D(Engine::DeferredRenderObject::G_BUFFER_DEPTH, 0, w, h);
	texture->SetGenerateMipMaps(false);
	texture->SetMemoryLayoutFormat(GL_DEPTH_COMPONENT32);
	texture->SetImageFormatType(GL_DEPTH_COMPONENT);
	texture->SetPixelFormatType(GL_FLOAT);

	Engine::TextureInstance* textureInstance = new Engine::TextureInstance(texture);
	textureInstance->SetMagnificationFilterType(GL_NEAREST);
	textureInstance->SetMinificationFilterType(GL_NEAREST);
	textureInstance->SetSComponentWrapType(GL_CLAMP_TO_EDGE);
	textureInstance->SetTComponentWrapType(GL_CLAMP_TO_EDGE);

	depthBuffer.bufferType = GL_DEPTH_ATTACHMENT;
	depthBuffer.texture = textureInstance;

	gBufferMap[Engine::DeferredRenderObject::G_BUFFER_DEPTH] = textureInstance;

	return textureInstance;
}

Engine::TextureInstance* Engine::DeferredRenderObject::GetBufferByName(std::string name)
{
	std::map<std::string, Engine::TextureInstance*>::iterator it = gBufferMap.find(name);
	if (it != gBufferMap.end())
	{
		return it->second;
	}
	return NULL;
}

void Engine::DeferredRenderObject::Initialize()
{
	glGenFramebuffers(1, &fbo);
	for (unsigned int i = 0; i < colorBuffersSize; i++)
	{
		colorBuffers[i].texture->GenerateTexture();
		colorBuffers[i].texture->ConfigureTexture();
	}

	depthBuffer.texture->GenerateTexture();
	depthBuffer.texture->ConfigureTexture();
}

void Engine::DeferredRenderObject::SetReSizeMod(float wm, float hm)
{
	widthMod = wm;
	heightMod = hm;
}

void Engine::DeferredRenderObject::ReSizeFBO(uint32 w, uint32 h)
{
	float moddedW = float(w) * widthMod;
	float moddedH = float(h) * heightMod;
	w = (unsigned int)ceil(moddedW);
	h = (unsigned int)ceil(moddedH);

	for (unsigned int i = 0; i < colorBuffersSize; i++)
	{
		colorBuffers[i].texture->ReSize(w, h);
	}

	depthBuffer.texture->ReSize(w, h);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	GLenum* buffers = new GLenum[colorBuffersSize];
	for (unsigned int i = 0; i < colorBuffersSize; i++)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, colorBuffers[i].bufferType, GL_TEXTURE_2D, colorBuffers[i].texture->GetTexture()->GetTextureID(), 0);
		buffers[i] = colorBuffers[i].bufferType;
	}

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer.texture->GetTexture()->GetTextureID(), 0);

	if (colorBuffersSize > 0)
	{
		glDrawBuffers(colorBuffersSize, buffers);
	}
	else
	{
		glDrawBuffer(GL_NONE);
	}

	if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER))
	{
		exit(-1);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Engine::DeferredRenderObject::PopulateDeferredObject(Engine::PostProcessObject* obj)
{
	for (uint32 i = 0; i < colorBuffersSize; i++)
	{
		obj->AddTexture("color_" + std::to_string(i), colorBuffers[i].texture);
	}
	if (renderDepth)
	{
		obj->AddTexture("depth", depthBuffer.texture);
	}
}
