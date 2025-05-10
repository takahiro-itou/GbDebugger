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

template  <typename  T>
inline  int
checkFlags(
        const  T  &     vExp,
        const  T  &     vAct,
        const  int      nLine)
{
    char    buf[512];
    if ( vExp != vAct ) {
        sprintf(buf,
                "\nEquality Assertion Failed."
                "\n  Expected : %08x"
                "\n  Actual   : %08x"
                "\nAt " __FILE__ ":%d\n",
                vExp, vAct, nLine);
        std::cerr   <<  buf;
        return ( 1 );
    }

    return ( 0 );
}

inline  int
runTestLogical(
        const  int      nLine,
        const  RegType  res,
        const  RegType  lhs,
        const  RegType  rhs,
        const  RegType  exp)
{
    const  RegType  act = setCondLogical(res, lhs, rhs, 0, 0);
    return  checkFlags(exp, act, nLine);
}

inline  int
runTestAdd(
        const  int      nLine,
        const  RegType  lhs,
        const  RegType  rhs,
        const  RegType  exp)
{
    const  RegType  act = setCondAdd(lhs + rhs, lhs, rhs, 0, 0);
    return  checkFlags(exp, act, nLine);
}

inline  int
runTestSub(
        const  int      nLine,
        const  RegType  lhs,
        const  RegType  rhs,
        const  RegType  exp)
{
    const  RegType  act = setCondSub(lhs - rhs, lhs, rhs, 0, 0);
    return  checkFlags(exp, act, nLine);
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
    int cnt = 0;
    cnt += runTestLogical(__LINE__, 10 & 3, 10, 3, 0);
    cnt += runTestLogical(__LINE__, 10 & 4, 10, 4, CPSR::FLAG_Z);
    cnt += runTestLogical(
                __LINE__, 0x7FFFFFFF | 0x80000000,
                0x7FFFFFFF, 0x80000000, CPSR::FLAG_N
    );

    CPPUNIT_ASSERT_EQUAL( 0, cnt );
    return;
}

void  CpuUtilsTest::testSetCondAdd()
{
    runTestAdd(__LINE__,  0,  0, CPSR::FLAG_Z);
    runTestAdd(__LINE__,  1, -1, CPSR::FLAG_Z | CPSR::FLAG_C);
    runTestAdd(__LINE__, -1,  1, CPSR::FLAG_Z | CPSR::FLAG_C);
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
