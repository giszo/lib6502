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
 *   - http://www.llx.com/~nparker/a2/opcodes.html
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

	    uint64_t m_ticks;
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

	// possible addressing modes
	enum AddressingMode
	{
	    Immediate,
	    ZeroPage,
	    ZeroPageX,
	    ZeroPageY,
	    Absolute,
	    AbsoluteX,
	    AbsoluteY,
	    IndirectX,
	    IndirectY
	};

	static const unsigned NumOfAddrModes = IndirectY + 1;

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

    private:
	void buildAddressingModeTable();

#ifdef HAVE_INSTRUCTION_TRACE
	void traceInstruction(const std::string& instr, const std::string& param = "");
#endif

	// reads a byte from the memory pointed by the PC and increments the PC
	uint8_t read8();
	// reads a word from the memory pointed by the PC and increments the PC by 2
	uint16_t read16();

	// pushes a byte onto the stack
	void push8(uint8_t data);
	// pops a byte from the stack
	uint8_t pop8();

	void performInterrupt(uint16_t vectorBase);

	// returns the addressing mode of the given instruction
	AddressingMode getAddressingMode(uint8_t opCode);

	uint16_t addrZero(std::string& trace);
	uint16_t addrZeroX(std::string& trace);
	uint16_t addrZeroY(std::string& trace);
	uint16_t addrAbsolute(std::string& trace);
	uint16_t addrAbsoluteX(std::string& trace);
	uint16_t addrAbsoluteY(std::string& trace);
	uint16_t addrIndirectX(std::string& trace);
	uint16_t addrIndirectY(std::string& trace);

	// sets or clears the mask on the status register regarding the given condition (true -> set, false -> clear)
	void setOrClearStatus(uint8_t flagMask, bool condition);

	void updateZero(uint8_t value);
	void updateSign(uint8_t value);

	unsigned bpl(uint8_t opCode);
	unsigned bcs(uint8_t opCode);
	unsigned bne(uint8_t opCode);
	unsigned beq(uint8_t opCode);
	unsigned bcc(uint8_t opCode);
	unsigned bmi(uint8_t opCode);
	unsigned bvc(uint8_t opCode);
	unsigned bvs(uint8_t opCode);

	unsigned jmpAbs(uint8_t opCode);
	unsigned jmpInd(uint8_t opCode);
	unsigned jsrAbs(uint8_t opCode);
	unsigned rts(uint8_t opCode);
	unsigned rti(uint8_t opCode);
	unsigned brk(uint8_t opCode);

	unsigned nop(uint8_t opCode);
	unsigned dop(uint8_t opCode);

	unsigned sei(uint8_t opCode);
	unsigned cli(uint8_t opCode);
	unsigned sed(uint8_t opCode);
	unsigned cld(uint8_t opCode);
	unsigned sec(uint8_t opCode);
	unsigned clc(uint8_t opCode);
	unsigned clv(uint8_t opCode);

	// stack instructions
	unsigned txs(uint8_t opCode);
	unsigned tsx(uint8_t opCode);
	unsigned pha(uint8_t opCode);
	unsigned pla(uint8_t opCode);
	unsigned php(uint8_t opCode);
	unsigned plp(uint8_t opCode);

	unsigned txa(uint8_t opCode);
	unsigned tax(uint8_t opCode);
	unsigned tya(uint8_t opCode);
	unsigned tay(uint8_t opCode);

	unsigned inc(uint8_t opCode);
	unsigned dec(uint8_t opCode);

	unsigned inx(uint8_t opCode);
	unsigned dex(uint8_t opCode);

	unsigned ldxImm(uint8_t opCode);
	unsigned ldxAddr(uint8_t opCode);
	unsigned stx(uint8_t opCode);

	unsigned iny(uint8_t opCode);
	unsigned dey(uint8_t opCode);

	unsigned ldyImm(uint8_t opCode);
	unsigned ldyAddr(uint8_t opCode);
	unsigned sty(uint8_t opCode);

	// ORA (bitwise OR with Accumulator)
	unsigned oraImm(uint8_t opCode);
	unsigned oraAddr(uint8_t opCode);

	unsigned eorImm(uint8_t opCode);
	unsigned eorAddr(uint8_t opCode);

	unsigned andImm(uint8_t opCode);
	unsigned andAddr(uint8_t opCode);

	unsigned ldaImm(uint8_t opCode);
	unsigned ldaAddr(uint8_t opCode);
	unsigned sta(uint8_t opCode);

	unsigned adcImm(uint8_t opCode);
	unsigned adcAddr(uint8_t opCode);
	unsigned sbcImm(uint8_t opCode);
	unsigned sbcAddr(uint8_t opCode);

	unsigned cpxImm(uint8_t opCode);
	unsigned cpxAddr(uint8_t opCode);

	unsigned cpyImm(uint8_t opCode);
	unsigned cpyAddr(uint8_t opCode);

	unsigned cmpImm(uint8_t opCode);
	unsigned cmpAddr(uint8_t opCode);

	unsigned bit(uint8_t opCode);

	unsigned aslAcc(uint8_t opCode);
	unsigned aslAddr(uint8_t opCode);
	unsigned lsrAcc(uint8_t opCode);
	unsigned lsrAddr(uint8_t opCode);

	unsigned rolAcc(uint8_t opCode);
	unsigned rolAddr(uint8_t opCode);
	unsigned rorAcc(uint8_t opCode);
	unsigned rorAddr(uint8_t opCode);

    private:
	// the actual state of the CPU
	State m_state;
	// the state of the CPU before executing the next instruction (used for tracing)
	State m_instrState;
	// reference to the memory used by the CPU
	Memory& m_memory;

	// addressing mode table
	typedef std::function<uint16_t(std::string&)> TAddressingMode;
	TAddressingMode m_addrModeTable[9];

	InstructionTracer* m_instrTracer;

	// remaining ticks of the current instruction before going to the next one
	unsigned m_remainingTicks;
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
