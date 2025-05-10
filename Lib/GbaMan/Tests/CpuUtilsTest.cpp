//  -*-  coding: utf-8-with-signature;  mode: c++  -*-  //
/*************************************************************************
**                                                                      **
**                  ----   Gb Debugger Project.   ----                  **
**                                                                      **
**          Copyright (C), 2025-2025, Takahiro Itou                     **
**          All Rights Reserved.                                        **
**                                                                      **
**          License: (See COPYING or LICENSE files)                     **
**          GNU Affero General Public License (AGPL) version 3,         **
**          or (at your option) any later version.                      **
**                                                                      **
*************************************************************************/

/**
**      An Implementation of Test Case 'CpuUtils'.
**
**      @file       Common/Tests/CpuUtils.cpp
**/

#include    "TestDriver.h"
#include    "GbDebugger/GbaMan/CpuUtils.h"

#include    "TestConf.h"


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

namespace  {

inline  void
runTestLogical(
        const  RegType  res,
        const  RegType  lhs,
        const  RegType  rhs,
        const  RegType  exp)
{
    CPPUNIT_ASSERT_EQUAL(exp, setCondLogical(res, lhs, rhs, 0, 0));
}

inline  void
runTestAdd(
        const  RegType  lhs,
        const  RegType  rhs,
        const  RegType  exp)
{
    CPPUNIT_ASSERT_EQUAL(exp, setCondAdd(lhs + rhs, lhs, rhs, 0, 0));
}

inline  void
runTestSub(
        const  RegType  lhs,
        const  RegType  rhs,
        const  RegType  exp)
{
    CPPUNIT_ASSERT_EQUAL(exp, setCondSub(lhs - rhs, lhs, rhs, 0, 0));
}

}   //  End of (Unnamed) namespace.

//========================================================================
//
//    CpuUtilsTest  class.
//
/**
**    クラス CpuUtils の単体テスト。
**/

class  CpuUtilsTest : public  TestFixture
{
    CPPUNIT_TEST_SUITE(CpuUtilsTest);
    CPPUNIT_TEST(testSetCondLogical);
    CPPUNIT_TEST(testSetCondAdd);
    CPPUNIT_TEST(testSetCondSub);
    CPPUNIT_TEST_SUITE_END();

public:
    virtual  void   setUp()     override    { }
    virtual  void   tearDown()  override    { }

private:
    void  testSetCondLogical();
    void  testSetCondAdd();
    void  testSetCondSub();
};

CPPUNIT_TEST_SUITE_REGISTRATION( CpuUtilsTest );

//========================================================================
//
//    Tests.
//

void  CpuUtilsTest::testSetCondLogical()
{
    runTestLogical(10 & 3, 10, 3, 0);
    runTestLogical(10 & 4, 10, 4, CPSR::FLAG_Z);
    runTestLogical(
            0x7FFFFFFF | 0x80000000, 0x7FFFFFFF, 0x80000000, CPSR::FLAG_N
    );
    return;
}

void  CpuUtilsTest::testSetCondAdd()
{
}

void  CpuUtilsTest::testSetCondSub()
{
}

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END

//========================================================================
//
//    エントリポイント。
//

int  main(int argc, char * argv[])
{
    return ( executeCppUnitTests(argc, argv) );
}
