#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

// =====================================================================================================================
void Cpu::lsrAcc()
{
    traceInstruction("LSR A");

    // bit #0 is shifted into carry
    setOrClearStatus(Carry, (m_A & 0x1) == 1);
    m_A >>= 1;
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::lsrZero()
{
    uint16_t addr = addrZero();
    traceInstruction(MakeString(true) << "LSR $" << std::setw(2) << std::setfill('0') << addr);

    uint8_t data = m_memory.read(addr);
    // bit #0 is shifted into carry
    setOrClearStatus(Carry, (data & 0x1) == 1);
    data >>= 1;
    updateZero(data);
    updateSign(data);
    m_memory.write(addr, data);
}

// =====================================================================================================================
void Cpu::lsrZeroX()
{
    uint8_t offset;
    uint16_t addr = addrZeroX(offset);
    traceInstruction(MakeString(true) << "LSR $" << std::setw(2) << std::setfill('0') << (int)offset << ",X");

    uint8_t data = m_memory.read(addr);
    // bit #0 is shifted into carry
    setOrClearStatus(Carry, (data & 0x1) == 1);
    data >>= 1;
    updateZero(data);
    updateSign(data);
    m_memory.write(addr, data);
}

// =====================================================================================================================
void Cpu::lsrAbs()
{
    uint16_t addr = addrAbsolute();
    traceInstruction(MakeString() << "LSR $" << std::hex << std::setw(4) << std::setfill('0') << addr);

    uint8_t data = m_memory.read(addr);
    // bit #0 is shifted into carry
    setOrClearStatus(Carry, (data & 0x1) == 1);
    data >>= 1;
    updateZero(data);
    updateSign(data);
    m_memory.write(addr, data);
}

// =====================================================================================================================
void Cpu::lsrAbsX()
{
    uint16_t abs;
    uint16_t addr = addrAbsoluteX(abs);
    traceInstruction(MakeString() << "LSR $" << std::hex << std::setw(4) << std::setfill('0') << abs << ",X");

    uint8_t data = m_memory.read(addr);
    // bit #0 is shifted into carry
    setOrClearStatus(Carry, (data & 0x1) == 1);
    data >>= 1;
    updateZero(data);
    updateSign(data);
    m_memory.write(addr, data);
}
