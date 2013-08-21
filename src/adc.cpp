#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

// =====================================================================================================================
void Cpu::adcImm()
{
    uint8_t imm = read8();
    traceInstruction(MakeString() << "ADC #$" << std::hex << std::setw(2) << std::setfill('0') << (int)imm);
    int result = m_A + imm + (m_status & Carry ? 1 : 0);
    m_A = result;
    updateZero(m_A);
    updateSign(m_A);
    setOrClearStatus(Carry, result > 255);
    // TODO: V status update
}

// =====================================================================================================================
void Cpu::adcZero()
{
    uint16_t addr = read8();
    traceInstruction(MakeString() << "ADC $" << std::hex << std::setw(4) << std::setfill('0') << addr);
    int result = m_A + m_memory.read(addr) + (m_status & Carry ? 1 : 0);
    m_A = result;
    updateZero(m_A);
    updateSign(m_A);
    setOrClearStatus(Carry, result > 255);
    // TODO: V status update
}

// =====================================================================================================================
void Cpu::adcZeroX()
{
    uint16_t addr = read8();
    traceInstruction(MakeString() << "ADC $" << std::hex << std::setw(4) << std::setfill('0') << addr << ",X");
    int result = m_A + m_memory.read(addr + m_Y) + (m_status & Carry ? 1 : 0);
    m_A = result;
    updateZero(m_A);
    updateSign(m_A);
    setOrClearStatus(Carry, result > 255);
    // TODO: V status update
}

// =====================================================================================================================
void Cpu::adcAbs()
{
    uint16_t addr = read16();
    traceInstruction(MakeString() << "ADC $" << std::hex << std::setw(4) << std::setfill('0') << addr);
    int result = m_A + m_memory.read(addr) + (m_status & Carry ? 1 : 0);
    m_A = result;
    updateZero(m_A);
    updateSign(m_A);
    setOrClearStatus(Carry, result > 255);
    // TODO: V status update
}

// =====================================================================================================================
void Cpu::adcAbsX()
{
    uint16_t addr = read16();
    traceInstruction(MakeString() << "ADC $" << std::hex << std::setw(4) << std::setfill('0') << addr << ",X");
    int result = m_A + m_memory.read(addr + m_Y) + (m_status & Carry ? 1 : 0);
    m_A = result;
    updateZero(m_A);
    updateSign(m_A);
    setOrClearStatus(Carry, result > 255);
    // TODO: V status update
}

// =====================================================================================================================
void Cpu::adcAbsY()
{
    uint16_t addr = read16();
    traceInstruction(MakeString() << "ADC $" << std::hex << std::setw(4) << std::setfill('0') << addr << ",Y");
    int result = m_A + m_memory.read(addr + m_Y) + (m_status & Carry ? 1 : 0);
    m_A = result;
    updateZero(m_A);
    updateSign(m_A);
    setOrClearStatus(Carry, result > 255);
    // TODO: V status update
}
