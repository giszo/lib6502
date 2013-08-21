#include <boost/test/unit_test.hpp>

#include "fixture.h"

using lib6502::Cpu;

// $0a
BOOST_FIXTURE_TEST_CASE(testAslAcc, Fixture)
{
    auto& cpuState = m_cpu->getState();

    // prepare test instruction
    m_ram[0x8000] = 0x0a;
    m_ram[0x8001] = 0x0a;

    // tick
    cpuState.m_A = 0x40;
    PREPARE_FLAGS(cpuState.m_status, Cpu::Carry, Cpu::Sign);
    m_cpu->tick();

    // check the result
    BOOST_CHECK_EQUAL(cpuState.m_A, 0x80);
    CHECK_FLAGS(cpuState.m_status, Cpu::Sign, Cpu::Carry);

    // tick
    cpuState.m_A = 0x80;
    PREPARE_FLAGS(cpuState.m_status, 0, Cpu::Carry | Cpu::Zero);
    m_cpu->tick();

    // check the result
    BOOST_CHECK_EQUAL(cpuState.m_A, 0x00);
    CHECK_FLAGS(cpuState.m_status, Cpu::Zero | Cpu::Carry, 0);
}
