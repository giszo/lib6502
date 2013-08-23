#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

// =====================================================================================================================
unsigned Cpu::sei(uint8_t opCode)
{
    traceInstruction("SEI");
    m_status |= IntDisable;
    return 2;
}

// =====================================================================================================================
unsigned Cpu::cli(uint8_t opCode)
{
    traceInstruction("CLI");
    m_status &= ~IntDisable;
    return 2;
}

// =====================================================================================================================
unsigned Cpu::sed(uint8_t opCode)
{
    traceInstruction("SED");
    m_status |= Decimal;
    return 2;
}

// =====================================================================================================================
unsigned Cpu::cld(uint8_t opCode)
{
    traceInstruction("CLD");
    m_status &= ~Decimal;
    return 2;
}

// =====================================================================================================================
unsigned Cpu::sec(uint8_t opCode)
{
    traceInstruction("SEC");
    m_status |= Carry;
    return 2;
}

// =====================================================================================================================
unsigned Cpu::clc(uint8_t opCode)
{
    traceInstruction("CLC");
    m_status &= ~Carry;
    return 2;
}

// =====================================================================================================================
unsigned Cpu::clv(uint8_t opCode)
{
    traceInstruction("CLV");
    m_status &= ~Overflow;
    return 2;
}
