#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

// =====================================================================================================================
unsigned Cpu::ldyImm(uint8_t opCode)
{
    m_Y = read8();
    traceInstruction("LDY", MakeString(true) << " #$" << std::setw(2) << std::setfill('0') << (int)m_Y);

    updateZero(m_Y);
    updateSign(m_Y);

    return 2;
}

// =====================================================================================================================
unsigned Cpu::ldyAddr(uint8_t opCode)
{
    std::string addrTrace;
    uint16_t address = m_addrModeTable[getAddressingMode(opCode)](addrTrace);

    traceInstruction("LDY", addrTrace);

    m_Y = m_memory.read(address);
    updateZero(m_Y);
    updateSign(m_Y);

    // TODO
    return 3;
}
