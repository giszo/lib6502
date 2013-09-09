#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

static const unsigned s_cpyTicks[Cpu::NumOfAddrModes] = {2, 3, 0, 0, 4, 0, 0, 0, 0};

// =====================================================================================================================
unsigned Cpu::cpyImm(uint8_t opCode)
{
    uint8_t imm = read8();

#ifdef HAVE_INSTRUCTION_TRACE
    traceInstruction("CPY", MakeString(true) << " #$" << std::setw(2) << std::setfill('0') << (int)imm);
#endif

    setOrClearStatus(Carry, m_Y >= imm);
    setOrClearStatus(Zero, m_Y == imm);
    setOrClearStatus(Sign, (m_Y - imm) & 0x80);

    return s_cpyTicks[Immediate];
}

// =====================================================================================================================
unsigned Cpu::cpyAddr(uint8_t opCode)
{
    auto addrMode = getAddressingMode(opCode);
    std::string addrTrace;
    uint16_t address = m_addrModeTable[addrMode](addrTrace);

#ifdef HAVE_INSTRUCTION_TRACE
    traceInstruction("CPY", addrTrace);
#endif

    uint8_t imm = m_memory.read(address);
    setOrClearStatus(Carry, m_Y >= imm);
    setOrClearStatus(Zero, m_Y == imm);
    setOrClearStatus(Sign, (m_Y - imm) & 0x80);

    return s_cpyTicks[addrMode];
}
