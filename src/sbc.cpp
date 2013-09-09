#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

#define SBC_CORE(s) \
    uint8_t _imm = (s); \
    unsigned result = m_A - _imm - (1 - (m_status & Carry ? 1 : 0));		\
    uint8_t origA = m_A; \
    m_A = result;	 \
    updateZero(m_A); \
    updateSign(m_A); \
    setOrClearStatus(Carry, result < 0x100); \
    setOrClearStatus(Overflow, (origA ^ _imm) & (origA ^ m_A) & 0x80);

static const unsigned s_sbcTicks[Cpu::NumOfAddrModes] = {2, 3, 4, 0, 4, 4, 4, 6, 5};

// =====================================================================================================================
unsigned Cpu::sbcImm(uint8_t addr)
{
    uint8_t imm = read8();

#ifdef HAVE_INSTRUCTION_TRACE
    traceInstruction("SBC", MakeString(true) << " #$" << std::setw(2) << std::setfill('0') << (int)imm);
#endif

    SBC_CORE(imm);
    return s_sbcTicks[Immediate];
}

// =====================================================================================================================
unsigned Cpu::sbcAddr(uint8_t opCode)
{
    auto addrMode = getAddressingMode(opCode);
    std::string addrTrace;
    uint16_t address = m_addrModeTable[addrMode](addrTrace);

#ifdef HAVE_INSTRUCTION_TRACE
    traceInstruction("SBC", addrTrace);
#endif

    SBC_CORE(m_memory.read(address));

    return s_sbcTicks[addrMode];
}
