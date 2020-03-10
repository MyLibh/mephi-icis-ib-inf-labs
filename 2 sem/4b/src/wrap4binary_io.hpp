#pragma once

#include <fstream>
#include <string>

template<typename _T>
class wrap4binary_io final
{
public:
	wrap4binary_io()              noexcept = default;
	wrap4binary_io(const _T& val) noexcept;
	wrap4binary_io(_T&&val)       noexcept;

	wrap4binary_io& operator=(const wrap4binary_io&) noexcept = default;
	wrap4binary_io& operator=(wrap4binary_io&&)      noexcept = default;

	__forceinline operator _T&() noexcept
	{
		return m_val;
	}

	__forceinline operator const _T&() const noexcept
	{
		return m_val;
	}

private:
	_T m_val;
};

template<typename _T>
std::ostream& operator<<(std::ostream& ostr, const wrap4binary_io<_T>& w4bio);

template<typename _T>
std::istream& operator>>(std::istream& istr, wrap4binary_io<_T>& w4bio);

template<typename _T>
inline wrap4binary_io<_T>::wrap4binary_io(const _T& val) noexcept :
	m_val(val)
{ }

template<typename _T>
inline wrap4binary_io<_T>::wrap4binary_io(_T&& val) noexcept :
	m_val(val)
{ }

template<typename _T>
std::ostream& operator<<(std::ostream& ostr, const wrap4binary_io<_T>& w4bio)
{
	ostr.write(reinterpret_cast<const char*>(&static_cast<const _T&>(w4bio)), sizeof(_T));

	return ostr;
}

template<typename _T>
std::istream& operator>>(std::istream& istr, wrap4binary_io<_T>& w4bio)
{
	istr.read(reinterpret_cast<char*>(&static_cast<_T&>(w4bio)), sizeof(_T));

	return istr;
}

template<>
std::ostream& operator<<<std::string>(std::ostream& ostr, const wrap4binary_io<std::string>& w4bio)
{
	size_t size{ static_cast<std::string>(w4bio).length() };

	ostr.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
	ostr.write(static_cast<std::string>(w4bio).c_str(), size);

	return ostr;
}

template<>
std::istream& operator>><std::string>(std::istream& istr, wrap4binary_io<std::string>& w4bio)
{
	size_t size = 0;
	istr.read(reinterpret_cast<char*>(&size), sizeof(size_t));

	std::unique_ptr<char[]> buf(new char[size + 1]);
	istr.read(buf.get(), size);
	buf[size] = '\0';

	w4bio = std::move(std::string(buf.get()));

	return istr;
}