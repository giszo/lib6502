#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

#define SBC_CORE(s) \
    uint8_t _imm = (s); \
    unsigned result = m_A - _imm - (1 - (m_status & Carry ? 1 : 0));		\
    uint8_t origA = m_A; \
    m_A = result;	 \
    updateZero(m_A); \
    updateSign(m_A); \
    setOrClearStatus(Carry, result < 0x100); \
    setOrClearStatus(Overflow, (origA ^ _imm) & (origA ^ m_A) & 0x80);

// =====================================================================================================================
void Cpu::sbcImm()
{
    uint8_t imm = read8();
    traceInstruction(MakeString() << "SBC #$" << std::hex << std::setw(2) << std::setfill('0') << (int)imm);
    SBC_CORE(imm);
}

// =====================================================================================================================
void Cpu::sbcZero()
{
    uint16_t address = addrZero();
    traceInstruction(MakeString(true) << "SBC $" << std::setw(2) << std::setfill('0') << address);

    SBC_CORE(m_memory.read(address));
}

// =====================================================================================================================
void Cpu::sbcZeroX()
{
    uint8_t offset;
    uint16_t address = addrZeroX(offset);
    traceInstruction(MakeString(true) << "SBC $" << std::setw(2) << std::setfill('0') << (int)offset << ",X");

    SBC_CORE(m_memory.read(address));
}

// =====================================================================================================================
void Cpu::sbcAbs()
{
    uint16_t addr = addrAbsolute();
    traceInstruction(MakeString(true) << "SBC $" << std::setw(4) << std::setfill('0') << addr);
    SBC_CORE(m_memory.read(addr));
}

// =====================================================================================================================
void Cpu::sbcAbsX()
{
    uint16_t abs;
    uint16_t addr = addrAbsoluteX(abs);
    traceInstruction(MakeString() << "SBC $" << std::hex << std::setw(4) << std::setfill('0') << abs << ",X");
    SBC_CORE(m_memory.read(addr));
}

// =====================================================================================================================
void Cpu::sbcAbsY()
{
    uint16_t abs;
    uint16_t addr = addrAbsoluteY(abs);
    traceInstruction(MakeString() << "SBC $" << std::hex << std::setw(4) << std::setfill('0') << abs << ",Y");
    SBC_CORE(m_memory.read(addr));
}

// =====================================================================================================================
void Cpu::sbcIndX()
{
    uint8_t offset;
    uint16_t address = addrIndirectX(offset);
    traceInstruction(MakeString(true) << "SBC ($" << std::setw(2) << std::setfill('0') << (int)offset << ",X)");

    SBC_CORE(m_memory.read(address));
}

// =====================================================================================================================
void Cpu::sbcIndY()
{
    uint8_t offset;
    uint16_t address = addrIndirectY(offset);
    traceInstruction(MakeString(true) << "SBC $(" << std::setw(2) << std::setfill('0') << (int)offset << "),Y");

    SBC_CORE(m_memory.read(address));
}
