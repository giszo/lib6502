#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

static const unsigned s_cmpTicks[Cpu::NumOfAddrModes] = {2, 3, 4, 0, 4, 4, 4, 6, 5};

// =====================================================================================================================
unsigned Cpu::cmpImm(uint8_t opCode)
{
    uint8_t imm = read8();

#ifdef HAVE_INSTRUCTION_TRACE
    traceInstruction("CMP", MakeString(true) << "#$" << std::setw(2) << std::setfill('0') << (int)imm);
#endif

    setOrClearStatus(Carry, m_A >= imm);
    setOrClearStatus(Zero, m_A == imm);
    setOrClearStatus(Sign, (m_A - imm) & 0x80);

    return s_cmpTicks[Immediate];
}

// =====================================================================================================================
unsigned Cpu::cmpAddr(uint8_t opCode)
{
    auto addrMode = getAddressingMode(opCode);
    std::string addrTrace;
    uint16_t address = m_addrModeTable[addrMode](addrTrace);

#ifdef HAVE_INSTRUCTION_TRACE
    traceInstruction("CMP", addrTrace);
#endif

    uint8_t imm = m_memory.read(address);
    setOrClearStatus(Carry, m_A >= imm);
    setOrClearStatus(Zero, m_A == imm);
    setOrClearStatus(Sign, (m_A - imm) & 0x80);

    return s_cmpTicks[addrMode];
}
