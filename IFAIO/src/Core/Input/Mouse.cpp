#include "Mouse.h"

#include "Mouse.h"
#include <Windows.h>

namespace IFAIO
{

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

	void Mouse::getAnyEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int width, int height, EventType& e)
	{
		switch (msg)
		{
		case WM_MOUSEMOVE:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			// in client region -> log move, and log enter + capture mouse (if not previously in window)
			if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height)
			{
				onMouseMove(pt.x, pt.y);
				if (!isInWindow())
				{
					SetCapture(hWnd);
					onMouseEnter();
				}
			}
			// not in client -> log move / maintain capture if button down
			else
			{
				if (wParam & (MK_LBUTTON | MK_RBUTTON))
				{
					onMouseMove(pt.x, pt.y);
				}
				// button up -> release capture / log event for leaving
				else
				{
					ReleaseCapture();
					onMouseLeave();
				}
			}
			e = EventType::MOUSE_MOTION;
			break;
		}
		case WM_LBUTTONDOWN:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			onLeftPressed(pt.x, pt.y);
			e = EventType::MOUSE_LEFT_BUTTON_PRESSED;
			break;
		}
		case WM_RBUTTONDOWN:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			onRightPressed(pt.x, pt.y);
			e = EventType::MOUSE_RIGHT_BUTTON_PRESSED;
			break;
		}
		case WM_LBUTTONUP:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			onLeftReleased(pt.x, pt.y);
			// release mouse if outside of window
			if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
			{
				ReleaseCapture();
				onMouseLeave();
			}
			e = EventType::MOUSE_LEFT_BUTTON_RELEASE;
			break;
		}
		case WM_RBUTTONUP:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			onRightReleased(pt.x, pt.y);
			// release mouse if outside of window
			if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
			{
				ReleaseCapture();
				onMouseLeave();
			}
			e = EventType::MOUSE_RIGHT_BUTTON_RELEASE;
			break;
		}
		case WM_MOUSEWHEEL:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
			onWheelDelta(pt.x, pt.y, delta);
			if (delta < 0)
				e = EventType::MOUSE_WHEEL_DOWN;
			else
				e = EventType::MOUSE_WHEEL_UP;
			break;
		}
		}
	}

}