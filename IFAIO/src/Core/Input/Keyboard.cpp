#include "Keyboard.h"

namespace IFAIO
{

	bool Keyboard::isKeyPressed(unsigned char Keycode) const noexcept
	{
		return m_Keystates[Keycode];
	}

	Keyboard::Event Keyboard::readKey() noexcept
	{
		if (m_Keybuffer.size() > 0u)
		{
			Keyboard::Event e = m_Keybuffer.front();
			m_Keybuffer.pop();
			return e;
		}
		else
		{
			return Keyboard::Event();
		}
	}

	bool Keyboard::isKeyEmpty() const noexcept
	{
		return m_Keybuffer.empty();
	}

	char Keyboard::readChar() noexcept
	{
		if (m_Charbuffer.size() > 0u)
		{
			unsigned char charcode = m_Charbuffer.front();
			m_Charbuffer.pop();
			return charcode;
		}
		else
		{
			return 0;
		}
	}

	bool Keyboard::isCharEmpty() const noexcept
	{
		return m_Charbuffer.empty();
	}

	void Keyboard::flushKey() noexcept
	{
		m_Keybuffer = std::queue<Event>();
	}

	void Keyboard::flushChar() noexcept
	{
		m_Charbuffer = std::queue<char>();
	}

	void Keyboard::flush() noexcept
	{
		flushKey();
		flushChar();
	}

	void Keyboard::enableAutorepeat() noexcept
	{
		m_AutorepeatEnabled = true;
	}

	void Keyboard::disableAutorepeat() noexcept
	{
		m_AutorepeatEnabled = false;
	}

	bool Keyboard::isAutorepeatEnabled() const noexcept
	{
		return m_AutorepeatEnabled;
	}

	void Keyboard::onKeyPressed(unsigned char m_Keycode) noexcept
	{
		m_Keystates[m_Keycode] = true;
		m_Keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Press, m_Keycode));
		trimBuffer(m_Keybuffer);
	}

	void Keyboard::onKeyReleased(unsigned char m_Keycode) noexcept
	{
		m_Keystates[m_Keycode] = false;
		m_Keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Release, m_Keycode));
		trimBuffer(m_Keybuffer);
	}

	void Keyboard::onChar(char character) noexcept
	{
		m_Charbuffer.push(character);
		m_CurrentCharacter = character;
		trimBuffer(m_Charbuffer);
	}

	void Keyboard::clearState() noexcept
	{
		m_Keystates.reset();
	}

	void Keyboard::getAnyEvent(UINT msg, WPARAM wParam, LPARAM lParam, EventType& e)
	{
		switch (msg)
		{
		case WM_KEYDOWN:
			// syskey commands need to be handled to track ALT key, (VK_MENU) and F10
		case WM_SYSKEYDOWN:
			if (!(lParam & 0x40000000) || isAutorepeatEnabled()) // filter autorepeat
			{
				onKeyPressed(static_cast<unsigned char>(wParam));
				e = EventType::KEY_PRESSED;
			}
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			onKeyReleased(static_cast<unsigned char>(wParam));
			e = EventType::KEY_RELEASE;
			break;
		case WM_CHAR:
			auto a = lParam >> 31;
			if (!a && (!(lParam & 0x40000000) || isAutorepeatEnabled()))
			{
				onChar(static_cast<unsigned char>(wParam));
				e = EventType::TEXT_INPUT;
			}

			break;
		}
	}

	template<typename T>
	void Keyboard::trimBuffer(std::queue<T>& buffer) noexcept
	{
		while (buffer.size() > s_BufferSize)
		{
			buffer.pop();
		}
	}

}