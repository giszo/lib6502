#include <boost/test/unit_test.hpp>

#include "fixture.h"

// $05
BOOST_FIXTURE_TEST_CASE(testOraZero, Fixture)
{
    auto& cpuState = m_cpu->getState();

    // ORA Zero
    m_ram[0x8000] = 0x05;
    m_ram[0x8001] = 0x42;
    m_ram[0x0042] = 0xaa;

    // perform the operation
    cpuState.m_A = 0x55;
    m_cpu->tick();

    // check the result
    BOOST_REQUIRE_EQUAL(m_memAccessHist.size(), 3);
    BOOST_CHECK(m_memAccessHist[0] == READ_AT(0x8000));
    BOOST_CHECK(m_memAccessHist[1] == READ_AT(0x8001));
    BOOST_CHECK(m_memAccessHist[2] == READ_AT(0x0042));
    BOOST_CHECK_EQUAL(cpuState.m_A, 0xff /* 0x55 | 0xaa */);
}
