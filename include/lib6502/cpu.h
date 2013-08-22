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
	    Always1    = 0x20,
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

	// addressing helpers
	uint8_t addrZero();
	uint8_t addrZeroX(uint8_t& offset);
	uint8_t addrZeroY(uint8_t& offset);
	uint16_t addrAbsolute();
	uint16_t addrAbsoluteX(uint16_t& abs);
	uint16_t addrAbsoluteY(uint16_t& abs);
	uint16_t addrIndirectX(uint8_t& offset);
	uint16_t addrIndirectY(uint8_t& offfset);

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
	void bvc();
	void bvs();

	void jmpAbs();
	void jmpInd();
	void jsrAbs();
	void rts();
	void rti();
	void nop();

	void sei();
	void sed();
	void cld();
	void sec();
	void clc();
	void clv();

	// stack instructions
	void txs();
	void tsx();
	void pha();
	void pla();
	void php();
	void plp();

	void txa();
	void tax();
	void tya();
	void tay();

	void incZero();
	void incZeroX();
	void incAbs();
	void incAbsX();

	void decZero();
	void decZeroX();
	void decAbs();
	void decAbsX();

	void inx();
	void dex();

	void ldxImm();
	void ldxZero();
	void ldxZeroY();
	void ldxAbs();
	void ldxAbsY();

	void stxZero();
	void stxZeroY();
	void stxAbs();

	void iny();
	void dey();

	void ldyImm();
	void ldyZero();
	void ldyZeroX();
	void ldyAbs();
	void ldyAbsX();

	void styZero();
	void styZeroX();
	void styAbs();

	// ORA (bitwise OR with Accumulator)
	void oraImm();
	void oraZero();
	void oraZeroX();
	void oraAbs();
	void oraAbsX();
	void oraAbsY();
	void oraIndX();
	void oraIndY();

	void eorImm();
	void eorZero();
	void eorZeroX();
	void eorAbs();
	void eorAbsX();
	void eorAbsY();
	void eorIndX();
	void eorIndY();

	void andImm();
	void andZero();
	void andZeroX();
	void andAbs();
	void andAbsX();
	void andAbsY();
	void andIndX();
	void andIndY();

	void ldaImm();
	void ldaZero();
	void ldaZeroX();
	void ldaAbs();
	void ldaAbsX();
	void ldaAbsY();
	void ldaIndX();
	void ldaIndY();

	void adcImm();
	void adcZero();
	void adcZeroX();
	void adcAbs();
	void adcAbsX();
	void adcAbsY();
	void adcIndX();
	void adcIndY();

	void sbcImm();
	void sbcZero();
	void sbcZeroX();
	void sbcAbs();
	void sbcAbsX();
	void sbcAbsY();
	void sbcIndX();
	void sbcIndY();

	void staZero();
	void staZeroX();
	void staAbs();
	void staAbsX();
	void staAbsY();
	void staIndX();
	void staIndY();

	void cpxImm();
	void cpxZero();
	void cpxAbs();

	void cpyImm();
	void cpyZero();
	void cpyAbs();

	void cmpImm();
	void cmpZero();
	void cmpZeroX();
	void cmpAbs();
	void cmpAbsX();
	void cmpAbsY();
	void cmpIndX();
	void cmpIndY();

	void bitZero();
	void bitAbs();

	void aslAcc();
	void aslZero();
	void aslZeroX();
	void aslAbs();
	void aslAbsX();

	void lsrAcc();
	void lsrZero();
	void lsrZeroX();
	void lsrAbs();
	void lsrAbsX();

	void rolAcc();
	void rolZero();
	void rolZeroX();
	void rolAbs();
	void rolAbsX();

	void rorAcc();
	void rorZero();
	void rorZeroX();
	void rorAbs();
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
