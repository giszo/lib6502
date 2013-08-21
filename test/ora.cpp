#include <boost/test/unit_test.hpp>

#include "fixture.h"

using lib6502::Cpu;

// $05
BOOST_FIXTURE_TEST_CASE(testOraZero, Fixture)
{
    auto& cpuState = m_cpu->getState();

    // ORA Zero
    m_ram[0x8000] = 0x05;
    m_ram[0x8001] = 0x42;
    m_ram[0x0042] = 0x00;

    m_ram[0x8002] = 0x05;
    m_ram[0x8003] = 0x43;
    m_ram[0x0043] = 0x80;

    // tick
    cpuState.m_A = 0x00;
    cpuState.m_status &= ~Cpu::Zero;
    cpuState.m_status |= Cpu::Sign;
    m_cpu->tick();

    // check the result
    BOOST_REQUIRE_EQUAL(m_memAccessHist.size(), 3);
    BOOST_CHECK(m_memAccessHist[0] == READ_AT(0x8000));
    BOOST_CHECK(m_memAccessHist[1] == READ_AT(0x8001));
    BOOST_CHECK(m_memAccessHist[2] == READ_AT(0x0042));
    BOOST_CHECK_EQUAL(cpuState.m_A, 0x00 /* 0x00 | 0x00 */);
    BOOST_CHECK(cpuState.m_status & Cpu::Zero);
    BOOST_CHECK((cpuState.m_status & Cpu::Sign) == 0);

    // tick
    cpuState.m_A = 0x08;
    cpuState.m_status |= Cpu::Zero;
    cpuState.m_status &= ~Cpu::Sign;
    m_cpu->tick();

    // check the result
    BOOST_CHECK_EQUAL(cpuState.m_A, 0x88 /* 0x80 | 0x08 */);
    BOOST_CHECK((cpuState.m_status & Cpu::Zero) == 0);
    BOOST_CHECK(cpuState.m_status & Cpu::Sign);
}
