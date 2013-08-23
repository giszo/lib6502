#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

// =====================================================================================================================
void Cpu::stxZero()
{
    uint16_t addr = addrZero();
    traceInstruction(MakeString(true) << "STX $" << std::setw(2) << std::setfill('0') << addr);
    m_memory.write(addr, m_X);
}

// =====================================================================================================================
void Cpu::stxZeroY()
{
    uint8_t offset;
    uint16_t addr = addrZeroY(offset);
    traceInstruction(MakeString(true) << "STX $" << std::setw(2) << std::setfill('0') << (int)offset << ",Y");
    m_memory.write(addr, m_X);
}

// =====================================================================================================================
void Cpu::stxAbs()
{
    uint16_t addr = addrAbsolute();
    traceInstruction(MakeString(true) << "STX $" << std::setw(4) << std::setfill('0') << addr);
    m_memory.write(addr, m_X);
}
