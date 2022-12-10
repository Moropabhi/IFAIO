#include "InputSystem.h"
#include "App.h"

namespace IFAIO
{

    bool InputSystem::isKeyPressed(const App* root, int key)
    {
        return root->m_RootWindow.m_Keyboard.isKeyPressed(key);
    }

    bool InputSystem::isKeyJustPressed(const App* root, int key)
    {
        return root->m_RootWindow.m_Keyboard.isKeyPressed(key);
    }

    bool InputSystem::isKeyReleased(const App* root, int key)
    {
        return !root->m_RootWindow.m_Keyboard.isKeyPressed(key);
    }

    std::string InputSystem::getStringFromScancode(const App* root, int scancode)
    {

        return std::string();
    }

    std::string InputSystem::getText(App* root)
    {
        Keyboard& kbd = root->m_RootWindow.m_Keyboard;
        std::string res;
        res.reserve(kbd.noOfChar());
        while (kbd.isCharEmpty())
        {
            res+=kbd.readChar();
        }
        return res;
    }
    std::wstring InputSystem::getTextW(App* root)
    {
        Keyboard& kbd = root->m_RootWindow.m_Keyboard;
        std::wstring res;
        for (int i = 0; kbd.isCharEmpty(); i++)
        {
            res+=kbd.readChar();
        }
        return res;
    }

    bool InputSystem::isMouseOnRect2D(const App* app, Rect box)
    {
        return box.isInside(getMousePos(app));
    }

    Vec2 InputSystem::getMousePos(const App* app)
    {
        return app->m_RootWindow.m_Mouse.getPos();
    }

    Vec2 InputSystem::getMousePosinRespectToVec2(const App* app, Vec2 vec)
    {
        return getMousePos(app);
    }

    bool InputSystem::isMouseButtonPressed(const App* app, uint8_t button)
    {
        return button? app->m_RootWindow.m_Mouse.isRightPressed():app->m_RootWindow.m_Mouse.isLeftPressed();
    }

    bool InputSystem::isMouseButtonPressedOnRect2D(const App*, Rect box, uint8_t button)
    {
        return false;
    }

    bool InputSystem::isMouseButtonJustPressedOnRect2D(const App*, Rect box, uint8_t button)
    {
        return false;
    }

    bool InputSystem::isMouseButtonReleased(const App* app, uint8_t button)
    {
        return !isMouseButtonPressed(app,button);
    }

    bool InputSystem::isMouseButtonReleasedOnRect2D(const App*, Rect box, uint8_t button)
    {
        return false;
    }

}