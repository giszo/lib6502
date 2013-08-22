#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

// =====================================================================================================================
void Cpu::bpl()
{
    int8_t off = static_cast<int8_t>(read8());
    traceInstruction(MakeString(true) << "BPL " << std::setw(4) << std::setfill('0') << (m_PC + off));

    if ((m_status & Sign) == 0)
	m_PC += off;
}

// =====================================================================================================================
void Cpu::bcs()
{
    int8_t off = static_cast<int8_t>(read8());
    traceInstruction(MakeString(true) << "BCS " << std::setw(4) << std::setfill('0') << (m_PC + off));

    if (m_status & Carry)
	m_PC += off;
}

// =====================================================================================================================
void Cpu::bne()
{
    int8_t off = static_cast<int8_t>(read8());
    traceInstruction(MakeString(true) << "BNE " << std::setw(4) << std::setfill('0') << (m_PC + off));

    if ((m_status & Zero) == 0)
	m_PC += off;
}

// =====================================================================================================================
void Cpu::beq()
{
    int8_t off = static_cast<int8_t>(read8());
    traceInstruction(MakeString(true) << "BEQ " << std::setw(4) << std::setfill('0') << (m_PC + off));

    if (m_status & Zero)
	m_PC += off;
}

// =====================================================================================================================
void Cpu::bcc()
{
    int8_t off = static_cast<int8_t>(read8());
    traceInstruction(MakeString(true) << "BCC " << std::setw(4) << std::setfill('0') << (m_PC + off));

    if ((m_status & Carry) == 0)
	m_PC += off;
}

// =====================================================================================================================
void Cpu::bmi()
{
    int8_t off = static_cast<int8_t>(read8());
    traceInstruction(MakeString(true) << "BMI " << std::setw(4) << std::setfill('0') << (m_PC + off));

    if (m_status & Sign)
	m_PC += off;
}

// =====================================================================================================================
void Cpu::bvc()
{
    int8_t off = static_cast<int8_t>(read8());
    traceInstruction(MakeString(true) << "BVC " << std::setw(4) << std::setfill('0') << (m_PC + off));

    if ((m_status & Overflow) == 0)
	m_PC += off;
}

// =====================================================================================================================
void Cpu::bvs()
{
    int8_t off = static_cast<int8_t>(read8());
    traceInstruction(MakeString(true) << "BVS " << std::setw(4) << std::setfill('0') << (m_PC + off));

    if (m_status & Overflow)
	m_PC += off;
}
