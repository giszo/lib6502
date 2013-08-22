#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

// =====================================================================================================================
void Cpu::styZero()
{
    uint16_t address = addrZero();
    traceInstruction(MakeString(true) << "STY $" << std::setw(2) << std::setfill('0') << address);
    m_memory.write(address, m_Y);
}

// =====================================================================================================================
void Cpu::styZeroX()
{
    uint8_t offset;
    uint16_t address = addrZeroX(offset);
    traceInstruction(MakeString(true) << "STY $" << std::setw(2) << std::setfill('0') << (int)offset << ",X");

    m_memory.write(address, m_Y);
}

// =====================================================================================================================
void Cpu::styAbs()
{
    uint16_t address = addrAbsolute();
    traceInstruction(MakeString(true) << "STY $" << std::setw(4) << std::setfill('0') << address);
    m_memory.write(address, m_Y);
}
