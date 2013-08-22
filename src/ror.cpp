#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

// =====================================================================================================================
void Cpu::rorAcc()
{
    traceInstruction("ROR A");

    bool setCarry = (m_A & 0x1) == 1;
    m_A >>= 1;
    if (m_status & Carry)
	m_A |= 0x80;
    setOrClearStatus(Carry, setCarry);
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::rorZero()
{
    uint16_t address = addrZero();
    traceInstruction(MakeString(true) << " ROR $" << std::setw(2) << std::setfill('0') << address);

    uint8_t data = m_memory.read(address);
    bool setCarry = (data & 0x1) == 1;
    data >>= 1;
    if (m_status & Carry)
	data |= 0x80;
    setOrClearStatus(Carry, setCarry);
    updateZero(data);
    updateSign(data);
    m_memory.write(address, data);
}

// =====================================================================================================================
void Cpu::rorZeroX()
{
    uint8_t offset;
    uint16_t address = addrZeroX(offset);
    traceInstruction(MakeString(true) << " ROR $" << std::setw(2) << std::setfill('0') << (int)offset << "X");

    uint8_t data = m_memory.read(address);
    bool setCarry = (data & 0x1) == 1;
    data >>= 1;
    if (m_status & Carry)
	data |= 0x80;
    setOrClearStatus(Carry, setCarry);
    updateZero(data);
    updateSign(data);
    m_memory.write(address, data);
}

// =====================================================================================================================
void Cpu::rorAbs()
{
    uint16_t address = addrAbsolute();
    traceInstruction(MakeString(true) << " ROR $" << std::setw(4) << std::setfill('0') << address);

    uint8_t data = m_memory.read(address);
    bool setCarry = (data & 0x1) == 1;
    data >>= 1;
    if (m_status & Carry)
	data |= 0x80;
    setOrClearStatus(Carry, setCarry);
    updateZero(data);
    updateSign(data);
    m_memory.write(address, data);
}

// =====================================================================================================================
void Cpu::rorAbsX()
{
    uint16_t abs;
    uint16_t address = addrAbsoluteX(abs);
    traceInstruction(MakeString(true) << " ROR $" << std::setw(4) << std::setfill('0') << abs << ",X");

    uint8_t data = m_memory.read(address);
    bool setCarry = (data & 0x1) == 1;
    data >>= 1;
    if (m_status & Carry)
	data |= 0x80;
    setOrClearStatus(Carry, setCarry);
    m_memory.write(address, data);
    updateZero(data);
    updateSign(data);
}
