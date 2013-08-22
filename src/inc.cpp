#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

// =====================================================================================================================
void Cpu::incZero()
{
    uint16_t address = addrZero();
    traceInstruction(MakeString(true) << "INC $" << std::setw(2) << std::setfill('0') << address);

    uint8_t data = m_memory.read(address);
    ++data;
    updateZero(data);
    updateSign(data);
    m_memory.write(address, data);
}

// =====================================================================================================================
void Cpu::incZeroX()
{
    uint8_t offset;
    uint16_t address = addrZeroX(offset);
    traceInstruction(MakeString(true) << "INC $" << std::setw(2) << std::setfill('0') << (int)offset << ",X");

    uint8_t data = m_memory.read(address);
    ++data;
    updateZero(data);
    updateSign(data);
    m_memory.write(address, data);
}

// =====================================================================================================================
void Cpu::incAbs()
{
    uint16_t address = addrAbsolute();
    traceInstruction(MakeString(true) << "INC $" << std::setw(4) << std::setfill('0') << address);

    uint8_t data = m_memory.read(address);
    ++data;
    updateZero(data);
    updateSign(data);
    m_memory.write(address, data);
}

// =====================================================================================================================
void Cpu::incAbsX()
{
    uint16_t abs;
    uint16_t address = addrAbsoluteX(abs);
    traceInstruction(MakeString(true) << "INC $" << std::setw(4) << std::setfill('0') << abs << ",X");

    uint8_t data = m_memory.read(address);
    ++data;
    updateZero(data);
    updateSign(data);
    m_memory.write(address, data);
}
