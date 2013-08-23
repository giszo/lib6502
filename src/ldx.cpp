#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

// =====================================================================================================================
unsigned Cpu::ldxImm(uint8_t opCode)
{
    m_X = read8();
    traceInstruction("LDX", MakeString(true) << "#$" << std::setw(2) << std::setfill('0') << (int)m_X);

    updateZero(m_X);
    updateSign(m_X);

    return 2;
}

// =====================================================================================================================
unsigned Cpu::ldxAddr(uint8_t opCode)
{
    std::string addrTrace;
    auto addrMode = getAddressingMode(opCode);
    if (addrMode == ZeroPageX) addrMode = ZeroPageY;
    else if (addrMode == AbsoluteX) addrMode = AbsoluteY;
    uint16_t address = m_addrModeTable[addrMode](addrTrace);

    traceInstruction("LDX", addrTrace);

    m_X = m_memory.read(address);
    updateZero(m_X);
    updateSign(m_X);

    // TODO
    return 3;
}
