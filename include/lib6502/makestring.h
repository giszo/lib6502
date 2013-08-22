#ifndef NEMU_MAKESTRING_H_INCLUDED
#define NEMU_MAKESTRING_H_INCLUDED

#include <sstream>
#include <iomanip>

namespace lib6502
{

class MakeString
{
    public:
	MakeString(bool hexFormat = false)
	{
	    if (hexFormat)
		m_stream << std::hex;
	}

	template<typename T>
	MakeString& operator<<(const T& t)
	{ m_stream << t; return *this; }

	operator std::string() const
	{ return m_stream.str(); }

    private:
	std::ostringstream m_stream;
};

}

#endif
