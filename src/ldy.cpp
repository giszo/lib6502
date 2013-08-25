#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

static const unsigned s_ldyTicks[Cpu::NumOfAddrModes] = {2, 3, 4, 0, 4, 4, 0, 0, 0};

// =====================================================================================================================
unsigned Cpu::ldyImm(uint8_t opCode)
{
    m_Y = read8();
    traceInstruction("LDY", MakeString(true) << "#$" << std::setw(2) << std::setfill('0') << (int)m_Y);

    updateZero(m_Y);
    updateSign(m_Y);

    return s_ldyTicks[Immediate];
}

// =====================================================================================================================
unsigned Cpu::ldyAddr(uint8_t opCode)
{
    auto addrMode = getAddressingMode(opCode);
    std::string addrTrace;
    uint16_t address = m_addrModeTable[addrMode](addrTrace);

    traceInstruction("LDY", addrTrace);

    m_Y = m_memory.read(address);
    updateZero(m_Y);
    updateSign(m_Y);

    return s_ldyTicks[addrMode];
}
