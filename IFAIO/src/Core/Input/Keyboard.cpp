#include "Keyboard.h"

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
	trimBuffer(m_Charbuffer);
}

void Keyboard::clearState() noexcept
{
	m_Keystates.reset();
}

template<typename T>
void Keyboard::trimBuffer(std::queue<T>& buffer) noexcept
{
	while (buffer.size() > s_BufferSize)
	{
		buffer.pop();
	}
}