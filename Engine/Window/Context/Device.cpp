#include "Device.h"
Engine::Device::Device()
{	
}

Engine::Device::~Device()
{
}

void Engine::Device::CreateCursor()
{
	m_cursor[CursorShape::ARROW] = glfwCreateStandardCursor(static_cast<int>(CursorShape::ARROW));
	m_cursor[CursorShape::IBEAM] = glfwCreateStandardCursor(static_cast<int>(CursorShape::IBEAM));
	m_cursor[CursorShape::CROSSHAIR] = glfwCreateStandardCursor(static_cast<int>(CursorShape::CROSSHAIR));
	m_cursor[CursorShape::HAND] = glfwCreateStandardCursor(static_cast<int>(CursorShape::HAND));
	m_cursor[CursorShape::HRESIZE] = glfwCreateStandardCursor(static_cast<int>(CursorShape::HRESIZE));
	m_cursor[CursorShape::VRESIZE] = glfwCreateStandardCursor(static_cast<int>(CursorShape::VRESIZE));
}

void Engine::Device::BindErrorCallback()
{
	auto errorCallback = [](int p_code, const char* p_desc)
	{
		Device::getInstance()->ErrorEvent.Invoke(static_cast<DeviceError>(p_code), p_desc);
	};
	glfwSetErrorCallback(errorCallback);
}

void Engine::Device::DestroyCursors()
{
	glfwDestroyCursor(m_cursor[CursorShape::ARROW]);
	glfwDestroyCursor(m_cursor[CursorShape::IBEAM]);
	glfwDestroyCursor(m_cursor[CursorShape::CROSSHAIR]);
	glfwDestroyCursor(m_cursor[CursorShape::HAND]);
	glfwDestroyCursor(m_cursor[CursorShape::HRESIZE]);
	glfwDestroyCursor(m_cursor[CursorShape::VRESIZE]);
}

void Engine::Device::OnInit()
{
	int initCode = glfwInit();

	if (initCode == GLFW_FALSE)
	{
		//LOG_ERROR("glfwInit error !!!\n");
		glfwTerminate();
	}
	else
	{
		CreateCursor();
	}
	BindErrorCallback();
	ErrorEvent.AddListenerID(std::bind(&Device::LogInfo,
		this, std::placeholders::_1, std::placeholders::_2));
}

void Engine::Device::OnEnd()
{
	DestroyCursors();
	glfwTerminate();
}

void Engine::Device::PollEvents() const
{
	glfwPollEvents();
}

void Engine::Device::LogInfo(DeviceError error, std::string p_desc)
{
	//switch (error)
	//{
	//case DeviceError::NOT_INITIALIZED:
	//case DeviceError::NO_CURRENT_CONTEXT:
	//case DeviceError::INVALID_ENUM:
	//	//LOG_ERROR(p_desc);
	//}
}

GLFWcursor* Engine::Device::GetCursorInstance(CursorShape shape)
{
	return m_cursor.at(shape);
}