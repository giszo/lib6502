#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

// =====================================================================================================================
void Cpu::txs()
{
    traceInstruction("TXS");
    m_SP = m_X;
}

// =====================================================================================================================
void Cpu::tsx()
{
    traceInstruction("TSX");
    m_X = m_SP;
    updateZero(m_X);
    updateSign(m_X);
}

// =====================================================================================================================
void Cpu::pha()
{
    traceInstruction("PHA");
    push8(m_A);
}

// =====================================================================================================================
void Cpu::pla()
{
    traceInstruction("PLA");
    m_A = pop8();
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::php()
{
    traceInstruction("PHP");
    push8(m_status | Break);
}

// =====================================================================================================================
void Cpu::plp()
{
    traceInstruction("PLP");
    m_status = pop8() & ~Break;
    // this bit is hardwired in the status register of the CPU, it cannot be "overwritten" with this instruction
    m_status |= Always1;
}
