#include "Mouse.h"

#include "Mouse.h"
#include <Windows.h>


std::pair<int, int> Mouse::getPos() const noexcept
{
	return { m_X,m_Y };
}

int Mouse::getPosX() const noexcept
{
	return m_X;
}

int Mouse::getPosY() const noexcept
{
	return m_Y;
}

bool Mouse::isInWindow() const noexcept
{
	return m_IsInWindow;
}

bool Mouse::isLeftPressed() const noexcept
{
	return m_IsLeftPressed;
}

bool Mouse::isRightPressed() const noexcept
{
	return m_IsRightPressed;
}

Mouse::Event Mouse::read() noexcept
{
	if (m_Buffer.size() > 0u)
	{
		Mouse::Event e = m_Buffer.front();
		m_Buffer.pop();
		return e;
	}
	else
	{
		return Mouse::Event();
	}
}

void Mouse::flush() noexcept
{
	m_Buffer = std::queue<Event>();
}

void Mouse::onMouseMove(int newx, int newy) noexcept
{
	m_X = newx;
	m_Y = newy;

	m_Buffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
	trimBuffer();
}

void Mouse::onMouseLeave() noexcept
{
	m_IsInWindow = false;
	m_Buffer.push(Mouse::Event(Mouse::Event::Type::Leave, *this));
	trimBuffer();
}

void Mouse::onMouseEnter() noexcept
{
	m_IsInWindow = true;
	m_Buffer.push(Mouse::Event(Mouse::Event::Type::Enter, *this));
	trimBuffer();
}

void Mouse::onLeftPressed(int x, int y) noexcept
{
	m_IsLeftPressed = true;

	m_Buffer.push(Mouse::Event(Mouse::Event::Type::LPress, *this));
	trimBuffer();
}

void Mouse::onLeftReleased(int x, int y) noexcept
{
	m_IsLeftPressed = false;

	m_Buffer.push(Mouse::Event(Mouse::Event::Type::LRelease, *this));
	trimBuffer();
}

void Mouse::onRightPressed(int x, int y) noexcept
{
	m_IsRightPressed = true;

	m_Buffer.push(Mouse::Event(Mouse::Event::Type::RPress, *this));
	trimBuffer();
}

void Mouse::onRightReleased(int x, int y) noexcept
{
	m_IsRightPressed = false;

	m_Buffer.push(Mouse::Event(Mouse::Event::Type::RRelease, *this));
	trimBuffer();
}

void Mouse::onWheelUp(int x, int y) noexcept
{
	m_Buffer.push(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
	trimBuffer();
}

void Mouse::onWheelDown(int x, int y) noexcept
{
	m_Buffer.push(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
	trimBuffer();
}

void Mouse::trimBuffer() noexcept
{
	while (m_Buffer.size() > s_BufferSize)
	{
		m_Buffer.pop();
	}
}

void Mouse::onWheelDelta(int x, int y, int delta) noexcept
{
	m_WheelDeltaCarry += delta;
	// generate events for every 120 
	while (m_WheelDeltaCarry >= WHEEL_DELTA)
	{
		m_WheelDeltaCarry -= WHEEL_DELTA;
		onWheelUp(x, y);
	}
	while (m_WheelDeltaCarry <= -WHEEL_DELTA)
	{
		m_WheelDeltaCarry += WHEEL_DELTA;
		onWheelDown(x, y);
	}
}