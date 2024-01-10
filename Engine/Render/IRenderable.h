#pragma once


namespace Engine
{
	enum RenderMode
	{
		RENDER_MODE_SHADED,
		RENDER_MODE_WIRE,
		RENDER_MODE_POINT
	};

	class IRenderable
	{

	public:
		virtual void NotifyRenderModeUpdate(RenderMode mode) = 0;
	};
}

