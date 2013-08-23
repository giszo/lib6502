#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

// =====================================================================================================================
unsigned Cpu::eorImm(uint8_t opCode)
{
    uint8_t imm = read8();
    traceInstruction("EOR", MakeString(true) << " #$" << std::setw(2) << std::setfill('0') << (int)imm);

    m_A ^= imm;
    updateZero(m_A);
    updateSign(m_A);

    return 2;
}

// =====================================================================================================================
unsigned Cpu::eorAddr(uint8_t opCode)
{
    std::string addrTrace;
    uint16_t address = m_addrModeTable[getAddressingMode(opCode)](addrTrace);

    traceInstruction("EOR", addrTrace);

    m_A ^= m_memory.read(address);
    updateZero(m_A);
    updateSign(m_A);

    // TODO
    return 3;
}
