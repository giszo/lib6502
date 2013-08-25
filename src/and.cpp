#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

static const unsigned s_andTicks[Cpu::NumOfAddrModes] = {2, 3, 4, 4, 4, 4, 6, 5};

// =====================================================================================================================
unsigned Cpu::andImm(uint8_t opCode)
{
    uint8_t imm = read8();
    traceInstruction("AND", MakeString(true) << " #$" << std::setw(2) << std::setfill('0') << (int)imm);

    m_A &= imm;
    updateZero(m_A);
    updateSign(m_A);

    return s_andTicks[Immediate];
}

// =====================================================================================================================
unsigned Cpu::andAddr(uint8_t opCode)
{
    auto addrMode = getAddressingMode(opCode);
    std::string addrTrace;
    uint16_t address = m_addrModeTable[addrMode](addrTrace);

    traceInstruction("AND", addrTrace);

    m_A &= m_memory.read(address);
    updateZero(m_A);
    updateSign(m_A);

    return s_andTicks[addrMode];
}
