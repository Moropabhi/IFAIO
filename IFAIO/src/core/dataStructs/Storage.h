#pragma once
#include <vector>
#include <iterator>
#include <cstddef>

template<typename T>
struct Iterator;

template<typename T>
class Storage
{
public:
	using iterator = Iterator<T>;

	Storage();
	//Storage(T* values);
	Storage(const T& value, size_t count = 1);
	Storage(T&& value, size_t count = 1);
	~Storage();

	const T& at(const int index) const
	{
		if (!(index < m_Size))
			throw std::exception("Out of range accessing element");
		return *((index > 0 ? p_Data : p_Data+m_Size) + index);
	}
	T& get(int index)
	{
		if (!((index < 0 ? -index-1 : index) < m_Size))
			throw std::exception("Out of range accessing element");
		return *((index>0?begin():end()) + index);
	}
	T& operator[](int i)
	{
		return get(i);
	}
	const T& operator[](int i) const 
	{
		return at(i);
	}


	void add(const T& data);
	void add(T&& data);
	void add(const T& data, size_t count);
	void add(T&& data, size_t count);

	void remove(int index);
	void remove(const T& m);
	void remove(T& m);

	size_t size() { return m_Size; }
	size_t capacity() { return m_ReservedSize; }
	void resize(size_t size);
	void reserve(size_t count);

	iterator begin() { return iterator(p_Data); }
	iterator end() { return iterator(p_Data+m_Size); }
	const iterator cbegin() const { return iterator(p_Data); }
	const iterator cend() const { return iterator(p_Data+m_Size); }

	int m_IncrementNum = 5;
private: 
	size_t m_Size= 0, m_ReservedSize= 5;
	T* p_Data;
};


template<typename T>
struct Iterator
{
	using iterator_category = std::forward_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = T;
	using pointer = T*;
	using reference = T&;


	Iterator(pointer ptr) :m_ptr(ptr) {}

	reference operator*() const { return *m_ptr; }
	pointer operator->() { return m_ptr; }

	// Prefix increment
	Iterator& operator++() { m_ptr++; return *this; }

	// Postfix increment
	Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

	Iterator operator+( int count) {
		return Iterator(m_ptr + count);
	}
	Iterator operator-( int count) {
		return Iterator(m_ptr - count);
	}
	Iterator operator-( Iterator& b) {
		return Iterator(m_ptr - b.m_ptr);
	}

	friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
	friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };


private:
	pointer m_ptr;

};

template<typename T>
inline Storage<T>::Storage()
	: m_Size(0),m_ReservedSize(m_IncrementNum), p_Data(new T[m_ReservedSize])
{
	
}
template<typename T>
inline Storage<T>::Storage(const T& value, size_t count)
	: m_Size(count),m_ReservedSize(count), p_Data(new T[m_Size])
{
	std::fill(begin(), end(), value);
}
template<typename T>
inline Storage<T>::Storage(T&& value, size_t count)
	: m_Size(count),m_ReservedSize(count), p_Data(new T[m_Size])
{
	std::fill(begin(), end(), value);
	std::vector<int>::iterator i;
}

template<typename T>
inline Storage<T>::~Storage()
{
	delete[] p_Data;
}

template<typename T>
inline void Storage<T>::add(const T& data)
{
	if (!(m_ReservedSize > m_Size))
	{
		reserve(m_Size + m_IncrementNum);
	}
	p_Data[m_Size] = data;
	m_Size++;
}

template<typename T>
inline void Storage<T>::add(T&& data)
{
	if (m_ReservedSize <= m_Size)
	{
		reserve(m_Size + m_IncrementNum);
	}
	p_Data[m_Size] = std::move(data);
	m_Size++;
}

template<typename T>
inline void Storage<T>::add(const T& data, size_t count)
{
	if (m_ReservedSize < m_Size+count)
	{
		reserve(m_Size +count + m_IncrementNum);
	}
	std::fill(end(), end()+count, data);
	m_Size += count;
}

template<typename T>
inline void Storage<T>::add(T&& data, size_t count)
{
	if (m_ReservedSize <= m_Size+count)
	{
		reserve(m_Size +count + m_IncrementNum);
	}
	std::fill(end(), end() + count, data);
	m_Size += count;
}

template<typename T>
inline void Storage<T>::remove(int index)
{
	p_Data[index] = std::move(p_Data[m_Size - 1]);
	m_Size--;
}

template<typename T>
inline void Storage<T>::remove(const T& m)
{
	*(std::find(begin(), end(), m))= std::move(p_Data[m_Size - 1]);
	m_Size--;
}
template<typename T>
inline void Storage<T>::remove(T& m)
{
	m= std::move(p_Data[m_Size - 1]);
	m_Size--;
}

template<typename T>
inline void Storage<T>::resize(size_t size)
{
	if (m_ReservedSize > size)
	{
		m_Size = size;
		return;
	}

	auto temp = p_Data;

	p_Data = new T[size];
	for (int i = 0; i < m_Size; i++)
		p_Data[i] = std::move(temp[i]);
	m_Size=size;
	m_ReservedSize = size;
	delete[] temp;


}

template<typename T>
inline void Storage<T>::reserve(size_t count)
{
	auto temp = p_Data;

	p_Data = new T[m_ReservedSize + count];
	for (int i = 0; i < m_Size; i++)
		p_Data[i] = std::move(temp[i]);
	m_ReservedSize += count;
	delete[] temp;
}
