#ifndef NEMU_MAKESTRING_H_INCLUDED
#define NEMU_MAKESTRING_H_INCLUDED

#include <sstream>

namespace lib6502
{

class MakeString
{
    public:
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
