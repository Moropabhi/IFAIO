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

}