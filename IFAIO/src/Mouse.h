#pragma once

#pragma once
#include <queue>

class Mouse
{
	friend class Window;
public:
	struct Event
	{
	public:
		enum class Type
		{
			LPress,
			LRelease,
			RPress,
			RRelease,
			WheelUp,
			WheelDown,
			Move,
			Enter,
			Leave,
			Invalid
		};
	private:
		Type type = Type::Invalid;
		bool m_IsLeftPressed = false;
		bool m_IsRightPressed = false;
		int m_X = 0;
		int m_Y = 0;
	public:
		Event() = default;
		Event(Type type, const Mouse& parent) noexcept
			:
			type(type),
			m_IsLeftPressed(parent.m_IsLeftPressed),
			m_IsRightPressed(parent.m_IsRightPressed),
			m_X(parent.m_X),
			m_Y(parent.m_Y)
		{}
		bool IsValid() const noexcept
		{
			return type != Type::Invalid;
		}
		Type GetType() const noexcept
		{
			return type;
		}
		std::pair<int, int> GetPos() const noexcept
		{

			return{ m_X,m_Y };
		}
		int GetPosX() const noexcept
		{
			return m_X;
		}
		int GetPosY() const noexcept
		{
			return m_Y;
		}
		bool isLeftPressed() const noexcept
		{
			return m_IsLeftPressed;
		}
		bool isRightPressed() const noexcept
		{
			return m_IsRightPressed;
		}
	};
public:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
	std::pair<int, int> getPos() const noexcept;
	int getPosX() const noexcept;
	int getPosY() const noexcept;
	bool isInWindow() const noexcept;
	bool isLeftPressed() const noexcept;
	bool isRightPressed() const noexcept;
	Mouse::Event read() noexcept;
	bool isEmpty() const noexcept
	{
		return m_Buffer.empty();
	}
	void flush() noexcept;
private:
	void onMouseMove(int x, int y) noexcept;
	void onMouseLeave() noexcept;
	void onMouseEnter() noexcept;
	void onLeftPressed(int x, int y) noexcept;
	void onLeftReleased(int x, int y) noexcept;
	void onRightPressed(int x, int y) noexcept;
	void onRightReleased(int x, int y) noexcept;
	void onWheelUp(int x, int y) noexcept;
	void onWheelDown(int x, int y) noexcept;
	void trimBuffer() noexcept;
	void onWheelDelta(int x, int y, int delta) noexcept;
private:
	static constexpr unsigned int s_BufferSize = 16u;
	int m_X=0;
	int m_Y=0;
	bool m_IsLeftPressed = false;
	bool m_IsRightPressed = false;
	bool m_IsInWindow = false;
	int m_WheelDeltaCarry = 0;
	std::queue<Mouse::Event> m_Buffer;
};