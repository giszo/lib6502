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

	virtual void trace(uint16_t PC, const std::string& inst) = 0;
};

class CpuException : public std::runtime_error
{
    public:
	CpuException(const std::string& error)
	    : runtime_error(error)
	{}
};

/**
 * Main class for the MOS 6502 processor.
 *
 * Instructions are implemented according to the following documents:
 *   - http://www.6502.org/tutorials/6502opcodes.html
 *   - http://www.obelisk.demon.co.uk/6502/reference.html
 */
class Cpu
{
    public:
	Cpu(Memory& memory);

	bool isInInterrupt() const;

	uint8_t getA() const { return m_A; }
	void setA(uint8_t data) { m_A = data; }

	void setInstructionTracer(InstructionTracer* t);

	/**
	 * Simulates one clock tick on the CPU.
	 */
	void tick();

	/**
	 * Performs a reset on the CPU.
	 */
	void reset();

	void nmi();

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
	void beq();
	void bcc();
	void bmi();

	void jmpAbs();
	void jmpInd();
	void jsrAbs();
	void rts();
	void rti();

	void sei();
	void cld();
	void sec();
	void clc();

	void txs();
	void txa();
	void tax();
	void tya();
	void tay();

	void pha();
	void pla();

	void incZero();
	void incAbs();
	void decZero();
	void decAbs();
	void decAbsX();

	void inx();
	void dex();
	void ldxImm();
	void ldxZero();
	void ldxAbs();
	void ldxAbsY();
	void stxZero();
	void stxAbs();

	void iny();
	void dey();
	void ldyImm();
	void ldyZero();
	void ldyAbs();
	void ldyAbsX();
	void styZero();
	void styAbs();

	void oraImm();
	void oraZero();
	void eorImm();
	void eorZero();
	void andImm();
	void andAbsX();

	void ldaImm();
	void ldaZero();
	void ldaAbs();
	void ldaAbsX();
	void ldaAbsY();
	void ldaIndY();

	void adcImm();
	void adcZero();
	void adcAbs();
	void adcAbsY();

	void sbcImm();
	void sbcAbsY();

	void staZero();
	void staAbs();
	void staAbsX();
	void staAbsY();
	void staIndY();

	void cpxImm();

	void cpyImm();

	void cmpImm();
	void cmpZero();
	void cmpAbs();
	void cmpAbsY();

	void bitZero();
	void bitAbs();

	void aslAcc();
	void lsrAcc();
	void lsrZero();
	void lsrAbs();
	void rolAcc();
	void rolZero();
	void rolAbs();
	void rorAcc();
	void rorAbsX();

    private:
	enum Flags
	{
	    CARRY       = 0x01,
	    ZERO        = 0x02,
	    INT_DISABLE = 0x04,
	    DECIMAL     = 0x08,
	    BREAK       = 0x10,
	    OVERFLOW_   = 0x40,
	    SIGN        = 0x80
	};

	uint8_t m_A;
	uint8_t m_X;
	uint8_t m_Y;
	uint8_t m_status;
	uint8_t m_SP;
	uint16_t m_PC;

	bool m_inInterrupt;

	Memory& m_memory;

	std::function<void()> m_instrTable[256];

	// program counter before fetching the next instruction
	uint16_t m_instrPC;
	InstructionTracer* m_instrTracer;
};

}

#endif
