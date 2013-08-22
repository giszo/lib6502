#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

// =====================================================================================================================
void Cpu::staZero()
{
    uint16_t address = addrZero();
    traceInstruction(MakeString(true) << "STA $" << std::setw(2) << std::setfill('0') << address);

    m_memory.write(address, m_A);
}

// =====================================================================================================================
void Cpu::staZeroX()
{
    uint8_t offset;
    uint16_t address = addrZeroX(offset);
    traceInstruction(MakeString(true) << "STA $" << std::setw(2) << std::setfill('0') << (int)offset << ",X");

    m_memory.write(address, m_A);
}

// =====================================================================================================================
void Cpu::staAbs()
{
    uint16_t address = addrAbsolute();
    traceInstruction(MakeString(true) << "STA $" << std::setw(4) << std::setfill('0') << address);

    m_memory.write(address, m_A);
}

// =====================================================================================================================
void Cpu::staAbsX()
{
    uint16_t abs;
    uint16_t address = addrAbsoluteX(abs);
    traceInstruction(MakeString(true) << "STA $" << std::setw(4) << std::setfill('0') << abs << ",X");

    m_memory.write(address, m_A);
}

// =====================================================================================================================
void Cpu::staAbsY()
{
    uint16_t abs;
    uint16_t address = addrAbsoluteY(abs);
    traceInstruction(MakeString(true) << "STA $" << std::setw(4) << std::setfill('0') << abs << ",Y");

    m_memory.write(address, m_A);
}

// =====================================================================================================================
void Cpu::staIndX()
{
    uint8_t offset;
    uint16_t address = addrIndirectX(offset);
    traceInstruction(MakeString(true) << "STA ($" << std::setw(2) << std::setfill('0') << (int)offset << ",X)");

    m_memory.write(address, m_A);
}

// =====================================================================================================================
void Cpu::staIndY()
{
    uint8_t offset;
    uint16_t address = addrIndirectY(offset);
    traceInstruction(MakeString(true) << "STA ($" << std::setw(2) << std::setfill('0') << (int)offset << "),Y");

    m_memory.write(address, m_A);
}
