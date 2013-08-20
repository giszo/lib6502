#ifndef LIB6502_CPU_H_INCLUDED
#define LIB6502_CPU_H_INCLUDED

#include "memory.h"

#include <ostream>
#include <functional>
#include <string>
#include <stdexcept>

namespace lib6502
{

class InstructionTracer
{
    public:
	virtual ~InstructionTracer()
	{}

	virtual void trace(const std::string& inst) = 0;
};

class CpuException : public std::runtime_error
{
    public:
	CpuException(const std::string& error)
	    : runtime_error(error)
	{}
};

class Cpu
{
    public:
	Cpu(Memory& memory);

	void setInstructionTracer(InstructionTracer* t);

	/**
	 * Simulates one clock tick on the CPU.
	 */
	void tick();

	/**
	 * Performs a reset on the CPU.
	 */
	void reset();

	void dumpState(std::ostream& s);

    private:
	void buildInstructionTable();
	void traceInstruction(const std::string& s);

	// reads a byte from the memory pointed by the PC and increments the PC
	uint8_t read8();
	// reads a word from the memory pointed by the PC and increments the PC by 2
	uint16_t read16();

	// pushes a byte onto the stack
	void push8(uint8_t data);
	// pops a byte from the stack
	uint8_t pop8();

	// sets or clears the mask on the status register regarding the given condition (true -> set, false -> clear)
	void setOrClearStatus(uint8_t flagMask, bool condition);

	void updateZero(uint8_t value);
	void updateSign(uint8_t value);

	void bpl();
	void bcs();
	void bne();

	void jmpAbs();
	void jsrAbs();
	void rts();

	void sei();
	void cld();

	void txs();
	void txa();

	void incAbs();

	void dex();
	void ldxImm();

	void iny();
	void dey();
	void ldyImm();

	void oraImm();
	void andImm();

	void ldaImm();
	void ldaAbs();
	void ldaAbsX();

	void stxZero();

	void staZero();
	void staAbs();
	void staAbsY();
	void staIndY();

	void cpxImm();

	void cpyImm();

	void cmpImm();

	void bitAbs();

    private:
	enum Flags
	{
	    CARRY       = 0x01,
	    ZERO        = 0x02,
	    INT_DISABLE = 0x04,
	    DECIMAL     = 0x08,
	    BREAK       = 0x10,
	    OVERFLOW    = 0x40,
	    SIGN        = 0x80
	};

	uint8_t m_A;
	uint8_t m_X;
	uint8_t m_Y;
	uint8_t m_status;
	uint8_t m_SP;
	uint16_t m_PC;

	Memory& m_memory;

	std::function<void()> m_instrTable[256];

	InstructionTracer* m_instrTracer;
};

}

#endif
