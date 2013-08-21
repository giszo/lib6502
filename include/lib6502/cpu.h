#ifndef LIB6502_CPU_H_INCLUDED
#define LIB6502_CPU_H_INCLUDED

#include "memory.h"

#include <ostream>
#include <functional>
#include <string>
#include <stdexcept>

namespace lib6502
{

class InstructionTracer;

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
	// structure representing the internal state of the 6502 CPU
	struct State
	{
	    State();

	    uint8_t m_A;
	    uint8_t m_X;
	    uint8_t m_Y;
	    uint8_t m_status;
	    uint8_t m_SP;
	    uint16_t m_PC;
	    bool m_inInterrupt;
	};

	// masks for the available status bits
	enum Flags
	{
	    Carry      = 0x01,
	    Zero       = 0x02,
	    IntDisable = 0x04,
	    Decimal    = 0x08,
	    Break      = 0x10,
	    Overflow   = 0x40,
	    Sign       = 0x80
	};

	Cpu(Memory& memory);

	bool isInInterrupt() const;

	State& getState();

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
	void oraZeroX();
	void oraAbs();
	void oraAbsX();
	void oraAbsY();

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
	// the actual state of the CPU
	State m_state;
	// the state of the CPU before executing the next instruction (used for tracing)
	State m_instrState;
	// reference to the memory used by the CPU
	Memory& m_memory;

	std::function<void()> m_instrTable[256];

	InstructionTracer* m_instrTracer;
};

class InstructionTracer
{
    public:
	virtual ~InstructionTracer()
	{}

	/**
	 * Callback for tracing one instruction.
	 *
	 * @param state the CPU state before fetching and executing the next instruction
	 * @param inst string representation of the instruction
	 */
	virtual void trace(const Cpu::State& state, const std::string& inst) = 0;
};

}

#endif
