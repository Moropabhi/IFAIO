#pragma once

#pragma once
#include <queue>
#include <bitset>

class Keyboard
{
	friend class Window;
public:
	struct Event
	{
	public:
		enum class Type
		{
			Invalid = -1,
			Release,
			Press,
		};
	public:
		Event() = default;
		Event(Type type, unsigned char code) noexcept
			:m_Type(type),m_Code(code)
		{}
		bool IsPress() const noexcept
		{
			return bool(m_Type);
		}
		bool IsRelease() const noexcept
		{
			return !(bool)m_Type;
		}
		bool IsValid() const noexcept
		{
			return m_Type != Type::Invalid;
		}
		unsigned char GetCode() const noexcept
		{
			return m_Code;
		}
	private:
		Type m_Type=Type::Invalid;
		unsigned char m_Code=0u;
	};
public:
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;
	// key event stuff
	bool isKeyPressed(unsigned char keycode) const noexcept;
	Event readKey() noexcept;
	bool isKeyEmpty() const noexcept;
	void flushKey() noexcept;
	// char event stuff
	char readChar() noexcept;
	bool isCharEmpty() const noexcept;
	void flushChar() noexcept;
	void flush() noexcept;
	// autorepeat control
	void enableAutorepeat() noexcept;
	void disableAutorepeat() noexcept;
	bool isAutorepeatEnabled() const noexcept;
private:
	void onKeyPressed(unsigned char keycode) noexcept;
	void onKeyReleased(unsigned char keycode) noexcept;
	void onChar(char character) noexcept;
	void clearState() noexcept;
	template<typename T>
	static void trimBuffer(std::queue<T>& buffer) noexcept;
private:
	static constexpr unsigned int s_NoKeys = 256u;
	static constexpr unsigned int s_BufferSize = 16u;
	bool m_AutorepeatEnabled = false;
	std::bitset<s_NoKeys> m_Keystates;
	std::queue<Event> m_Keybuffer;
	std::queue<char> m_Charbuffer;
};

