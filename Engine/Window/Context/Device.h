#pragma once
#include "../../Util/Singleton.h"
#include"../../Event/Event.h"
#include <GLFW/glfw3.h>

namespace Engine
{
	enum class DeviceError
	{
		NOT_INITIALIZED = 0x00010001,
		NO_CURRENT_CONTEXT = 0x00010002,
		INVALID_ENUM = 0x00010003,
		INVALID_VALUE = 0x00010004,
		OUT_OF_MEMORY = 0x00010005,
		API_UNAVAILABLE = 0x00010006,
		VERSION_UNAVAILABLE = 0x00010007,
		PLATFORM_ERROR = 0x00010008,
		FORMAT_UNAVAILABLE = 0x00010009,
		NO_WINDOW_CONTEXT = 0x0001000A
	};

	enum class CursorShape
	{
		ARROW = 0x00036001,
		IBEAM = 0x00036002,
		CROSSHAIR = 0x00036003,
		HAND = 0x00036004,
		HRESIZE = 0x00036005,
		VRESIZE = 0x00036006
	};

	class Device : public CSingleton<Device>
	{
	private:
		Event<DeviceError, std::string> ErrorEvent;
		std::unordered_map<CursorShape, GLFWcursor*> m_cursor;

	public:
		Device();
		~Device();

		void CreateCursor();
		void BindErrorCallback();
		void DestroyCursors();
		void PollEvents() const;
		void LogInfo(DeviceError error_Code, std::string p_desc);

		void OnInit();
		void OnEnd();

		GLFWcursor* GetCursorInstance(CursorShape shape);

	};

}

