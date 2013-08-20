#ifndef LIB6502_MEMORY_H_INCLUDED
#define LIB6502_MEMORY_H_INCLUDED

#include <inttypes.h>

namespace lib6502
{

class Memory
{
    public:
	virtual ~Memory()
	{}

	virtual uint8_t read(uint16_t address) = 0;
	virtual void write(uint16_t address, uint8_t data) = 0;
};

}

#endif
