#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

// =====================================================================================================================
void Cpu::aslAcc()
{
    traceInstruction("ASL A");

    // bit #0 is shifted into carry
    setOrClearStatus(Carry, (m_A & 0x80) == 0x80);
    m_A <<= 1;
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::aslZero()
{
    uint16_t address = addrZero();
    traceInstruction(MakeString(true) << "ASL $" << std::setw(2) << std::setfill('0') << address);

    uint8_t data = m_memory.read(address);
    // bit #0 is shifted into carry
    setOrClearStatus(Carry, (data & 0x80) == 0x80);
    data <<= 1;
    updateZero(data);
    updateSign(data);
    m_memory.write(address, data);
}

// =====================================================================================================================
void Cpu::aslZeroX()
{
    uint8_t offset;
    uint16_t address = addrZeroX(offset);
    traceInstruction(MakeString(true) << "ASL $" << std::setw(2) << std::setfill('0') << (int)offset << ",X");

    uint8_t data = m_memory.read(address);
    // bit #0 is shifted into carry
    setOrClearStatus(Carry, (data & 0x80) == 0x80);
    data <<= 1;
    updateZero(data);
    updateSign(data);
    m_memory.write(address, data);
}

// =====================================================================================================================
void Cpu::aslAbs()
{
    uint16_t address = addrAbsolute();
    traceInstruction(MakeString(true) << "ASL $" << std::setw(4) << std::setfill('0') << address);

    uint8_t data = m_memory.read(address);
    // bit #0 is shifted into carry
    setOrClearStatus(Carry, (data & 0x80) == 0x80);
    data <<= 1;
    updateZero(data);
    updateSign(data);
    m_memory.write(address, data);
}

// =====================================================================================================================
void Cpu::aslAbsX()
{
    uint16_t abs;
    uint16_t address = addrAbsoluteX(abs);
    traceInstruction(MakeString(true) << "ASL $" << std::setw(4) << std::setfill('0') << abs);

    uint8_t data = m_memory.read(address);
    // bit #0 is shifted into carry
    setOrClearStatus(Carry, (data & 0x80) == 0x80);
    data <<= 1;
    updateZero(data);
    updateSign(data);
    m_memory.write(address, data);
}
