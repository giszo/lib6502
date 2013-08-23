#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

// =====================================================================================================================
unsigned Cpu::bpl(uint8_t opCode)
{
    int8_t off = static_cast<int8_t>(read8());
    traceInstruction("BPL", MakeString(true) << "$" << std::setw(4) << std::setfill('0') << (m_PC + off));

    if ((m_status & Sign) == 0)
	m_PC += off;

    // TODO
    return 2;
}

// =====================================================================================================================
unsigned Cpu::bcs(uint8_t opCode)
{
    int8_t off = static_cast<int8_t>(read8());
    traceInstruction("BCS", MakeString(true) << "$" << std::setw(4) << std::setfill('0') << (m_PC + off));

    if (m_status & Carry)
	m_PC += off;

    // TODO
    return 2;
}

// =====================================================================================================================
unsigned Cpu::bne(uint8_t opCode)
{
    int8_t off = static_cast<int8_t>(read8());
    traceInstruction("BNE", MakeString(true) << "$" << std::setw(4) << std::setfill('0') << (m_PC + off));

    if ((m_status & Zero) == 0)
	m_PC += off;

    // TODO
    return 2;
}

// =====================================================================================================================
unsigned Cpu::beq(uint8_t opCode)
{
    int8_t off = static_cast<int8_t>(read8());
    traceInstruction("BEQ", MakeString(true) << "$" << std::setw(4) << std::setfill('0') << (m_PC + off));

    if (m_status & Zero)
	m_PC += off;

    // TODO
    return 2;
}

// =====================================================================================================================
unsigned Cpu::bcc(uint8_t opCode)
{
    int8_t off = static_cast<int8_t>(read8());
    traceInstruction("BCC", MakeString(true) << "$" << std::setw(4) << std::setfill('0') << (m_PC + off));

    if ((m_status & Carry) == 0)
	m_PC += off;

    // TODO
    return 2;
}

// =====================================================================================================================
unsigned Cpu::bmi(uint8_t opCode)
{
    int8_t off = static_cast<int8_t>(read8());
    traceInstruction("BMI", MakeString(true) << "$" << std::setw(4) << std::setfill('0') << (m_PC + off));

    if (m_status & Sign)
	m_PC += off;

    // TODO
    return 2;
}

// =====================================================================================================================
unsigned Cpu::bvc(uint8_t opCode)
{
    int8_t off = static_cast<int8_t>(read8());
    traceInstruction("BVC", MakeString(true) << "$" << std::setw(4) << std::setfill('0') << (m_PC + off));

    if ((m_status & Overflow) == 0)
	m_PC += off;

    // TODO
    return 2;
}

// =====================================================================================================================
unsigned Cpu::bvs(uint8_t opCode)
{
    int8_t off = static_cast<int8_t>(read8());
    traceInstruction("BVS", MakeString(true) << "$" << std::setw(4) << std::setfill('0') << (m_PC + off));

    if (m_status & Overflow)
	m_PC += off;

    // TODO
    return 2;
}
