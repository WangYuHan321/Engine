#include "WindowManager.h"
#include "GLFW/glfw3.h"
#include "Context/Device.h"


Engine::Window::WindowManager::WindowManager()
{
}

void Engine::Window::WindowManager::CreateGLFWWindow(uint32 width, uint32 height)
{
	glfwWindowHint(GLFW_RESIZABLE, true);
	glfwWindowHint(GLFW_DECORATED, true);
	glfwWindowHint(GLFW_FOCUSED, true);
	glfwWindowHint(GLFW_MAXIMIZED, false);
	//ÊÇ·ñÖÃ¶¥
	glfwWindowHint(GLFW_FLOATING, false);
	glfwWindowHint(GLFW_VISIBLE, true);
	glfwWindowHint(GLFW_AUTO_ICONIFY, true);
	glfwWindowHint(GLFW_REFRESH_RATE, 60);
	glfwWindowHint(GLFW_SAMPLES, 12);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	m_glfwWindowSize.x = (float)width;
	m_glfwWindowSize.y = (float)height;

	m_glfwWindow = glfwCreateWindow(m_glfwWindowSize.x, m_glfwWindowSize.y,
		"GameEngine -- by WangYuHan", nullptr, nullptr);

	glfwMakeContextCurrent(m_glfwWindow);
	//glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Engine::Window::WindowManager::OnResize(uint32 width, uint32 height)
{
	m_glfwWindowSize.x = width;
	m_glfwWindowSize.y = height;
}

void Engine::Window::WindowManager::SwapBuffer()
{
	glfwSwapBuffers(m_glfwWindow);
}

void Engine::Window::WindowManager::OnInit(uint32 width, uint32 height)
{
	CreateGLFWWindow(width, height);
	BindKeyCallback();
	BindMouseCallback();
	BindCloseCallback();
	BindResizeCallback();
	BindMousePosCallback();
	BindFrameBufferCallback();
	/*fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
	fprintf(stdout, "Status: Using GLFW %s\n", glfwGetVersionString());*/

	ReSizeWindowEvent.AddListenerID(std::bind(&WindowManager::OnResize,
		this, std::placeholders::_1, std::placeholders::_2));
}

void Engine::Window::WindowManager::OnEnd()
{
	glfwDestroyWindow(m_glfwWindow);
}

void Engine::Window::WindowManager::SetCursorShape(CursorShape p_cursorShape)
{
	m_cursorShape = p_cursorShape;
	glfwSetCursor(m_glfwWindow, Device::getInstance()->GetCursorInstance(p_cursorShape));
}

void Engine::Window::WindowManager::BindKeyCallback()
{
	auto keyReleaseCallback = [](GLFWwindow* p_window, int p_key, int p_scanCode, int p_action, int p_modes)
	{
		if (p_action == GLFW_PRESS)
			WindowManager::getInstance()->KeyDownEvent.Invoke(p_key);
		if (p_action == GLFW_RELEASE)
			WindowManager::getInstance()->KeyReleaseEvent.Invoke(p_key);
	};
	glfwSetKeyCallback(GetWindow(), keyReleaseCallback);
}

void Engine::Window::WindowManager::BindMouseCallback()
{
	auto mouseCallback = [](GLFWwindow* p_window, int p_button, int p_action, int p_modes)
	{
		if (p_action == GLFW_PRESS)
			WindowManager::getInstance()->MouseButtonDownEvent.Invoke(p_button);
		if (p_action == GLFW_RELEASE)
			WindowManager::getInstance()->MouseButtonReleaseEvent.Invoke(p_button);

	};
	glfwSetMouseButtonCallback(GetWindow(), mouseCallback);
}

void Engine::Window::WindowManager::BindCloseCallback()
{
	auto closeCallback = [](GLFWwindow* p_window)
	{
		WindowManager::getInstance()->CloseEvent.Invoke();
	};
	glfwSetWindowCloseCallback(GetWindow(), closeCallback);
}

void Engine::Window::WindowManager::BindResizeCallback()
{
	auto resizeCallback = [](GLFWwindow* p_window, int p_width, int p_height)
	{
		WindowManager::getInstance()->ReSizeWindowEvent.Invoke(p_width, p_height);
	};
	glfwSetWindowSizeCallback(GetWindow(), resizeCallback);
}

void Engine::Window::WindowManager::BindMousePosCallback()
{
	auto mouseMoveCallback = [](GLFWwindow* p_widnow, double x, double y)
	{
		WindowManager::getInstance()->MousePosEvent.Invoke(x, y);
	};
	glfwSetCursorPosCallback(GetWindow(), mouseMoveCallback);
}

void Engine::Window::WindowManager::BindFrameBufferCallback()
{
	auto frameBufferCallback = [](GLFWwindow* p_widnow, int x, int y)
	{
		WindowManager::getInstance()->FrameBufferEvent.Invoke(x, y);
	};
	glfwSetFramebufferSizeCallback(GetWindow(), frameBufferCallback);
}

bool Engine::Window::WindowManager::ShouldClose() const
{
	return glfwWindowShouldClose(m_glfwWindow);
}

void Engine::Window::WindowManager::SetWindowSize(glm::vec2 size)
{
	m_glfwWindowSize = size;
	glfwSetWindowSize(m_glfwWindow, m_glfwWindowSize.x, m_glfwWindowSize.y);
}

void Engine::Window::WindowManager::SetShouldClose(bool p_value) const
{
	return glfwSetWindowShouldClose(m_glfwWindow, p_value);
}

Engine::Window::WindowManager::~WindowManager()
{
	CloseEvent.RemoveAllListeners();
	MousePosEvent.RemoveAllListeners();
	MinimizeEvent.RemoveAllListeners();
	MaximizeEvent.RemoveAllListeners();
	KeyDownEvent.RemoveAllListeners();
	KeyReleaseEvent.RemoveAllListeners();
	FrameBufferEvent.RemoveAllListeners();
	ReSizeWindowEvent.RemoveAllListeners();
	MouseButtonDownEvent.RemoveAllListeners();
	MouseButtonReleaseEvent.RemoveAllListeners();
}