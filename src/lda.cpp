#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

// =====================================================================================================================
unsigned Cpu::ldaImm(uint8_t opCode)
{
    m_A = read8();
    traceInstruction("LDA", MakeString(true) << " #$" << std::setw(2) << std::setfill('0') << (int)m_A);

    updateZero(m_A);
    updateSign(m_A);

    return 2;
}

// =====================================================================================================================
unsigned Cpu::ldaAddr(uint8_t opCode)
{
    std::string addrTrace;
    uint16_t address = m_addrModeTable[getAddressingMode(opCode)](addrTrace);

    traceInstruction("LDA", addrTrace);

    m_A = m_memory.read(address);
    updateZero(m_A);
    updateSign(m_A);

    // TODO
    return 3;
}
