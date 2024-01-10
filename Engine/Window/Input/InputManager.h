#pragma once

//���������ö������
#include "Key.h"
#include "MouseBtn.h"

#include "../../Util/common.h"
#include "../../Util/Singleton.h"
#include "../../Event/Event.h"
#include <GLFW/glfw3.h>

namespace Engine 
{
	namespace Window
	{
		namespace Input
		{
			class InputManager : public CSingleton<InputManager>
			{
			public:

				InputManager();
				~InputManager();

				void OnMouseRelease(uint32 mouse_btn);
				void OnMouseDown(uint32 mouse_btn);
				void OnKeyRelease(uint32 key_btn);
				void OnKeyDown(uint32 key_btn);

				bool IsKeyPressed(Input::EKey p_key);
				bool IsKeyReleased(Input::EKey p_key);
				bool IsMouseButtonPressed(EMouseButton mouse_btn);
				bool IsMouseButtonReleased(EMouseButton mouse_btn);

				void OnStartUp();
				void OnEnd();

				std::pair<double, double> GetMousePosition() const;

				void ClearEvents();

			private:
				listenerID m_keyDownListener;
				listenerID m_keyReleaseListener;
				listenerID m_mouseButtonDownListener;
				listenerID m_mouseButtonReleaseListener;

				std::unordered_map<Input::EKey, Input::EKeyState> m_keyEvents;
				std::unordered_map<EMouseButton, EMouseButtonState> m_mouseState;
			};
		}

	}
	
}



