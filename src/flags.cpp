#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

// =====================================================================================================================
unsigned Cpu::sei(uint8_t opCode)
{
#ifdef HAVE_INSTRUCTION_TRACE
    traceInstruction("SEI");
#endif
    m_status |= IntDisable;
    return 2;
}

// =====================================================================================================================
unsigned Cpu::cli(uint8_t opCode)
{
#ifdef HAVE_INSTRUCTION_TRACE
    traceInstruction("CLI");
#endif
    m_status &= ~IntDisable;
    return 2;
}

// =====================================================================================================================
unsigned Cpu::sed(uint8_t opCode)
{
#ifdef HAVE_INSTRUCTION_TRACE
    traceInstruction("SED");
#endif
    m_status |= Decimal;
    return 2;
}

// =====================================================================================================================
unsigned Cpu::cld(uint8_t opCode)
{
#ifdef HAVE_INSTRUCTION_TRACE
    traceInstruction("CLD");
#endif
    m_status &= ~Decimal;
    return 2;
}

// =====================================================================================================================
unsigned Cpu::sec(uint8_t opCode)
{
#ifdef HAVE_INSTRUCTION_TRACE
    traceInstruction("SEC");
#endif
    m_status |= Carry;
    return 2;
}

// =====================================================================================================================
unsigned Cpu::clc(uint8_t opCode)
{
#ifdef HAVE_INSTRUCTION_TRACE
    traceInstruction("CLC");
#endif
    m_status &= ~Carry;
    return 2;
}

// =====================================================================================================================
unsigned Cpu::clv(uint8_t opCode)
{
#ifdef HAVE_INSTRUCTION_TRACE
    traceInstruction("CLV");
#endif
    m_status &= ~Overflow;
    return 2;
}
