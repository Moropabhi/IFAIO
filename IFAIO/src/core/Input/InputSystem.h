#pragma once
#include "core/dataStructs/Utilities.h"
#include "Core/Input/Keyboard.h"
#include "Core/Input/Mouse.h"
namespace IFAIO
{
	class App;

	class InputSystem
	{

	public:
		// keyboard input handling//


			/*void setAction(std::string action, std::array<int , 3> key);

			bool isActionPressed();
			bool isActionJustPressed();
			bool isActionReleased();*/

		static bool isKeyPressed(const App* root, int key);
		static bool isKeyJustPressed(const App* root, int key);
		static bool isKeyReleased(const App* root, int key);

		static std::string getStringFromScancode(const App* root, int scancode);
		static std::string getText(App* root);
		static std::wstring getTextW(App* root);

		// mouse input handling //

			/*static bool isMouseOnRect2D(Rect2D box);
			static Vector2 getMousePos();
			static Vector2 getMousePosinRespectToRect2D(Rect2D box);

			static bool isMouseButtonPressed(uint8_t button);
			static bool isMouseButtonPressedOnRect2D(Rect2D box, uint8_t button);
			static bool isMouseButtonJustPressedOnRect2D(Rect2D box, uint8_t button);

			static bool isMouseButtonReleased(uint8_t button);
			static bool isMouseButtonReleasedOnRect2D(Rect2D box, uint8_t button);*/


	};
}