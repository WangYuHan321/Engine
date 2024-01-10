#pragma once

#include "../Program/Program.h"
#include "../Program/PostProcessProgram/PostProcessProgram.h"
#include "../Renderer.h"
#include "DeferredCallback.h"
#include "../DeferredRender/DeferredRenderObject.h"

namespace Engine
{
	struct PostProcessChainNode
	{
		Program* postProcessProgram;
		PostProcessObject* obj;
		DeferredRenderObject* renderBuffer;

		DeferredCallback* callBack;
	}typedef PostProcessChainNode;

	class DeferredRenderer : public Renderer
	{
	private:
		//场景中的前向渲染RT
		DeferredRenderObject* forwardPassBuffer;
		//后处理RT
		// 1 color buffer
		// 1 emission buffer
		//1 general-purpose info
		DeferredRenderObject* deferredPassBuffer;

		PostProcessProgram* deferredShading;

		PostProcessProgram* screenOutput;

		PostProcessObject* deferredDrawSurface;

		PostProcessObject* chainEnd;

		//debug
		Program* debugScreenOutput;

		PostProcessObject* debugObj;

		//debug

		std::list<PostProcessChainNode*> postProcessChain;
		
		bool initialized;

		void (DeferredRenderer::* renderFunc)();

		TextureInstance* gBufferPos;//位置
		TextureInstance* gBufferNormal;//法线
		TextureInstance* gBufferEmissive;//发光
		TextureInstance* gBufferSpecular;//
		TextureInstance* gBufferColor;
		TextureInstance* gBufferDepth;
		TextureInstance* gBufferInfo;

	public:
		DeferredRenderer();
		~DeferredRenderer();
		
		virtual void Initialize();
		void DoRender();
		void OnReSize(uint32 w, uint32 h);

		inline const Engine::TextureInstance* GetGBufferDepth()
		{
			return gBufferDepth;
		}

		inline const Engine::TextureInstance* GetGBufferPos()
		{
			return gBufferPos;
		}

		inline const Engine::TextureInstance* GetGBufferNormal()
		{
			return gBufferNormal;
		}

		inline const Engine::TextureInstance* GetGBufferSpecular()
		{
			return gBufferSpecular;
		}

		inline const Engine::TextureInstance* GetGBufferInfo()
		{
			return gBufferInfo;
		}

		void InitializeLoop();

		void AddPostProcess(PostProcessChainNode* object);
		void RunPostProcess();
	};
}


