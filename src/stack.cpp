#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

// =====================================================================================================================
unsigned Cpu::txs(uint8_t opCode)
{
    traceInstruction("TXS");
    m_SP = m_X;
    return 2;
}

// =====================================================================================================================
unsigned Cpu::tsx(uint8_t opCode)
{
    traceInstruction("TSX");
    m_X = m_SP;
    updateZero(m_X);
    updateSign(m_X);
    return 2;
}

// =====================================================================================================================
unsigned Cpu::pha(uint8_t opCode)
{
    traceInstruction("PHA");
    push8(m_A);
    return 3;
}

// =====================================================================================================================
unsigned Cpu::pla(uint8_t opCode)
{
    traceInstruction("PLA");
    m_A = pop8();
    updateZero(m_A);
    updateSign(m_A);
    return 4;
}

// =====================================================================================================================
unsigned Cpu::php(uint8_t opCode)
{
    traceInstruction("PHP");
    push8(m_status | Break);
    return 3;
}

// =====================================================================================================================
unsigned Cpu::plp(uint8_t opCode)
{
    traceInstruction("PLP");
    m_status = pop8() & ~Break;
    // this bit is hardwired in the status register of the CPU, it cannot be "overwritten" with this instruction
    m_status |= Always1;
    return 4;
}
