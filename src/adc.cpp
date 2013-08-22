#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

// =====================================================================================================================
void Cpu::adcImm()
{
    uint8_t imm = read8();
    traceInstruction(MakeString(true) << "ADC #$" << std::setw(2) << std::setfill('0') << (int)imm);

    int result = m_A + imm + (m_status & Carry ? 1 : 0);
    uint8_t origA = m_A;
    m_A = result;
    updateZero(m_A);
    updateSign(m_A);
    setOrClearStatus(Carry, result > 255);
    setOrClearStatus(Overflow, (origA ^ m_A) & (imm ^ m_A) & 0x80);
}

// =====================================================================================================================
void Cpu::adcZero()
{
    uint16_t address = addrZero();
    traceInstruction(MakeString(true) << "ADC $" << std::setw(2) << std::setfill('0') << address);

    uint8_t imm = m_memory.read(address);
    int result = m_A + imm + (m_status & Carry ? 1 : 0);
    uint8_t origA = m_A;
    m_A = result;
    updateZero(m_A);
    updateSign(m_A);
    setOrClearStatus(Carry, result > 255);
    setOrClearStatus(Overflow, (origA ^ m_A) & (imm ^ m_A) & 0x80);
}

// =====================================================================================================================
void Cpu::adcZeroX()
{
    uint8_t offset;
    uint16_t address = addrZeroX(offset);
    traceInstruction(MakeString(true) << "ADC $" << std::setw(2) << std::setfill('0') << (int)offset << ",X");

    uint8_t imm = m_memory.read(address);
    int result = m_A + imm + (m_status & Carry ? 1 : 0);
    uint8_t origA = m_A;
    m_A = result;
    updateZero(m_A);
    updateSign(m_A);
    setOrClearStatus(Carry, result > 255);
    setOrClearStatus(Overflow, (origA ^ m_A) & (imm ^ m_A) & 0x80);
}

// =====================================================================================================================
void Cpu::adcAbs()
{
    uint16_t address = addrAbsolute();
    traceInstruction(MakeString(true) << "ADC $" << std::setw(4) << std::setfill('0') << address);

    uint8_t imm = m_memory.read(address);
    int result = m_A + imm + (m_status & Carry ? 1 : 0);
    uint8_t origA = m_A;
    m_A = result;
    updateZero(m_A);
    updateSign(m_A);
    setOrClearStatus(Carry, result > 255);
    setOrClearStatus(Overflow, (origA ^ m_A) & (imm ^ m_A) & 0x80);
}

// =====================================================================================================================
void Cpu::adcAbsX()
{
    uint16_t abs;
    uint16_t address = addrAbsoluteX(abs);
    traceInstruction(MakeString(true) << "ADC $" << std::setw(4) << std::setfill('0') << abs << ",X");

    uint8_t imm = m_memory.read(address);
    int result = m_A + imm + (m_status & Carry ? 1 : 0);
    uint8_t origA = m_A;
    m_A = result;
    updateZero(m_A);
    updateSign(m_A);
    setOrClearStatus(Carry, result > 255);
    setOrClearStatus(Overflow, (origA ^ m_A) & (imm ^ m_A) & 0x80);
}

// =====================================================================================================================
void Cpu::adcAbsY()
{
    uint16_t abs;
    uint16_t address = addrAbsoluteY(abs);
    traceInstruction(MakeString(true) << "ADC $" << std::setw(4) << std::setfill('0') << abs << ",Y");

    uint8_t imm = m_memory.read(address);
    int result = m_A + imm + (m_status & Carry ? 1 : 0);
    uint8_t origA = m_A;
    m_A = result;
    updateZero(m_A);
    updateSign(m_A);
    setOrClearStatus(Carry, result > 255);
    setOrClearStatus(Overflow, (origA ^ m_A) & (imm ^ m_A) & 0x80);
}

// =====================================================================================================================
void Cpu::adcIndX()
{
    uint8_t offset;
    uint16_t address = addrIndirectX(offset);
    traceInstruction(MakeString() << "ADC ($" << std::hex << std::setw(2) << std::setfill('0') << (int)offset << ",X)");

    uint8_t imm = m_memory.read(address);
    int result = m_A + imm + (m_status & Carry ? 1 : 0);
    uint8_t origA = m_A;
    m_A = result;
    updateZero(m_A);
    updateSign(m_A);
    setOrClearStatus(Carry, result > 255);
    setOrClearStatus(Overflow, (origA ^ m_A) & (imm ^ m_A) & 0x80);
}

// =====================================================================================================================
void Cpu::adcIndY()
{
    uint8_t offset;
    uint16_t address = addrIndirectY(offset);
    traceInstruction(MakeString(true) << "ADC $(" << std::setw(2) << std::setfill('0') << (int)offset << "),Y");

    uint8_t imm = m_memory.read(address);
    int result = m_A + imm + (m_status & Carry ? 1 : 0);
    uint8_t origA = m_A;
    m_A = result;
    updateZero(m_A);
    updateSign(m_A);
    setOrClearStatus(Carry, result > 255);
    setOrClearStatus(Overflow, (origA ^ m_A) & (imm ^ m_A) & 0x80);
}
