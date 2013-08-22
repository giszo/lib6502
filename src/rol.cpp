#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

// =====================================================================================================================
void Cpu::rolAcc()
{
    traceInstruction("ROL A");

    // bit #0 is shifted into carry
    bool setCarry = m_A & 0x80;
    m_A <<= 1;
    if (m_status & Carry)
	m_A |= 1;
    setOrClearStatus(Carry, setCarry);
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::rolZero()
{
    uint16_t addr = addrZero();
    traceInstruction(MakeString(true) << "ROL $" << std::setw(2) << std::setfill('0') << addr);

    uint8_t data = m_memory.read(addr);
    // bit #0 is shifted into carry
    bool setCarry = data & 0x80;
    data <<= 1;
    if (m_status & Carry)
	data |= 1;
    setOrClearStatus(Carry, setCarry);
    updateZero(data);
    updateSign(data);
    m_memory.write(addr, data);
}

// =====================================================================================================================
void Cpu::rolZeroX()
{
    uint8_t offset;
    uint16_t address = addrZeroX(offset);
    traceInstruction(MakeString(true) << "ROL $" << std::setw(2) << std::setfill('0') << (int)offset << ",X");

    uint8_t data = m_memory.read(address);
    // bit #0 is shifted into carry
    bool setCarry = data & 0x80;
    data <<= 1;
    if (m_status & Carry)
	data |= 1;
    setOrClearStatus(Carry, setCarry);
    updateZero(data);
    updateSign(data);
    m_memory.write(address, data);
}

// =====================================================================================================================
void Cpu::rolAbs()
{
    uint16_t addr = addrAbsolute();
    traceInstruction(MakeString(true) << "ROL $" << std::setw(4) << std::setfill('0') << addr);

    uint8_t data = m_memory.read(addr);
    // bit #0 is shifted into carry
    bool setCarry = data & 0x80;
    data <<= 1;
    if (m_status & Carry)
	data |= 1;
    setOrClearStatus(Carry, setCarry);
    updateZero(data);
    updateSign(data);
    m_memory.write(addr, data);
}

// =====================================================================================================================
void Cpu::rolAbsX()
{
    uint16_t abs;
    uint16_t addr = addrAbsoluteX(abs);
    traceInstruction(MakeString(true) << "ROL $" << std::setw(4) << std::setfill('0') << abs << ",X");

    uint8_t data = m_memory.read(addr);
    // bit #0 is shifted into carry
    bool setCarry = data & 0x80;
    data <<= 1;
    if (m_status & Carry)
	data |= 1;
    setOrClearStatus(Carry, setCarry);
    updateZero(data);
    updateSign(data);
    m_memory.write(addr, data);
}
