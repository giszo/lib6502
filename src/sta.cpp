#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

// =====================================================================================================================
void Cpu::staZero()
{
    uint16_t addr = read8();
    traceInstruction(MakeString() << "STA $" << std::hex << std::setw(2) << std::setfill('0') << addr);
    m_memory.write(addr, m_A);
}

// =====================================================================================================================
void Cpu::staZeroX()
{
    uint16_t addr = read8();
    traceInstruction(MakeString() << "STA $" << std::hex << std::setw(2) << std::setfill('0') << addr << ",X");
    m_memory.write(addr + m_X, m_A);
}

// =====================================================================================================================
void Cpu::staAbs()
{
    uint16_t abs = read16();
    traceInstruction(MakeString() << "STA $" << std::hex << std::setw(4) << std::setfill('0') << abs);
    m_memory.write(abs, m_A);
}

// =====================================================================================================================
void Cpu::staAbsX()
{
    uint16_t abs = read16();
    traceInstruction(MakeString() << "STA $" << std::hex << std::setw(4) << std::setfill('0') << abs << ",X");
    m_memory.write(abs + m_X, m_A);
}

// =====================================================================================================================
void Cpu::staAbsY()
{
    uint16_t abs = read16();
    traceInstruction(MakeString() << "STA $" << std::hex << std::setw(4) << std::setfill('0') << abs << ",Y");
    m_memory.write(abs + m_Y, m_A);
}

// =====================================================================================================================
void Cpu::staIndY()
{
    uint8_t off = read8();
    traceInstruction(MakeString() << "STA ($" << std::hex << std::setw(2) << std::setfill('0') << off << "),Y");

    uint16_t addr = (m_memory.read(off) | (m_memory.read(off + 1) << 8)) + m_Y;
    m_memory.write(addr, m_A);
}
