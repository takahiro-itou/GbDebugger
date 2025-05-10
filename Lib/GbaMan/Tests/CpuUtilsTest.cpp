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
    const  RegType  act = setCondAdd(lhs + rhs, lhs, rhs, 0);
    return  checkFlags(exp, act, nLine);
}

inline  int
runTestSub(
        const  int      nLine,
        const  RegType  lhs,
        const  RegType  rhs,
        const  RegType  exp)
{
    const  RegType  act = setCondSub(lhs - rhs, lhs, rhs, 0);
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
    int cnt = 0;

    cnt += runTestAdd(__LINE__,  0,  0, CPSR::FLAG_Z);
    cnt += runTestAdd(__LINE__,  1, -1, CPSR::FLAG_Z | CPSR::FLAG_C);
    cnt += runTestAdd(__LINE__, -1,  1, CPSR::FLAG_Z | CPSR::FLAG_C);

    //  境界値テスト。  //
    //  2147483646 + 1 =  2147483647  : ----    //
    cnt += runTestAdd(__LINE__, 0x7FFFFFFE, 1, 0);
    cnt += runTestAdd(__LINE__, 1, 0x7FFFFFFE, 0);

    //  2147483646 + 2 = -2147483648  : N--V    //
    cnt += runTestAdd(__LINE__, 0x7FFFFFFE, 2, CPSR::FLAG_N | CPSR::FLAG_V);
    cnt += runTestAdd(__LINE__, 2, 0x7FFFFFFE, CPSR::FLAG_N | CPSR::FLAG_V);

    //  符号なし整数のキャリー  //

    //  4294967294 + 1 = 4294967295   : N---    //
    cnt += runTestAdd(__LINE__, 0xFFFFFFFE, 1, CPSR::FLAG_N);
    cnt += runTestAdd(__LINE__, 1, 0xFFFFFFFE, CPSR::FLAG_N);

    //  4294967294 + 2 = 0            : -ZC-    //
    cnt += runTestAdd(__LINE__, 0xFFFFFFFE, 2, CPSR::FLAG_Z | CPSR::FLAG_C);
    cnt += runTestAdd(__LINE__, 2, 0xFFFFFFFE, CPSR::FLAG_Z | CPSR::FLAG_C);

    //  4294967294 + 3 = 1            : --C-    //
    cnt += runTestAdd(__LINE__, 0xFFFFFFFE, 3, CPSR::FLAG_C);
    cnt += runTestAdd(__LINE__, 3, 0xFFFFFFFE, CPSR::FLAG_C);

    //  4294967295 + 4294967295 = 4294967294  : N-C-    //
    cnt += runTestAdd(
                __LINE__, 0xFFFFFFFF, 0xFFFFFFFF,
                CPSR::FLAG_N | CPSR::FLAG_C);

    CPPUNIT_ASSERT_EQUAL( 0, cnt );
    return;
}

void  CpuUtilsTest::testSetCondSub()
{
    int cnt = 0;

    //  繰り下がりがない時に C=1  に注意。  //

    cnt += runTestSub(__LINE__,  0,  0, CPSR::FLAG_Z | CPSR::FLAG_C);
    cnt += runTestSub(__LINE__,  1,  1, CPSR::FLAG_Z | CPSR::FLAG_C);
    cnt += runTestSub(__LINE__, -1, -1, CPSR::FLAG_Z | CPSR::FLAG_C);

    //  2 - 1 =  1 : --C-   //
    //  1 - 2 = -1 : N---   //
    cnt += runTestSub(__LINE__,  2,  1, CPSR::FLAG_C);
    cnt += runTestSub(__LINE__,  1,  2, CPSR::FLAG_N);

    //  0 - 2147483647 = -2147483647  : N---    //
    cnt += runTestSub(__LINE__, 0, 2147483647, CPSR::FLAG_N);

    //  2147483646 - 4294967295 = 2147483647    //
    //  2147483646 - (-1) =  2147483647 : ----  //
    cnt += runTestSub(__LINE__, 0x7FFFFFFE, 0xFFFFFFFF, 0);

    //  2147483646 - 4294967294 = 2147483648    //
    //  2147483646 - (-2) = -2147483648 : N--V  //
    cnt += runTestSub(
                __LINE__, 0x7FFFFFFE, 0xFFFFFFFE,
                CPSR::FLAG_N |CPSR::FLAG_V);

    //   2147483649 - 4294967295 = 2147483650   //
    //  -2147483647 - (-1) = -2147483648 : N--- //
    cnt += runTestSub(__LINE__, 0x80000001, 0xFFFFFFFF, CPSR::FLAG_N);

    //   2147483646 - 4294967295 = 2147483647   //
    //   2147483646 - (-1) = 2147483647 : ----  //
    cnt += runTestSub(__LINE__, 0x7FFFFFFE, 0xFFFFFFFF, 0);

    //   2147483646 - 4294967294 = 2147483648   //
    //   2147483646 - (-2) = -2147483648 : N--V //
    cnt += runTestSub(
                __LINE__, 0x7FFFFFFE, 0xFFFFFFFE,
                CPSR::FLAG_N | CPSR::FLAG_V);

    //   2147483649 - 1 = 2147483648            //
    //  -2147483647 - 1 = -2147483648 : N-C-    //
    cnt += runTestSub(__LINE__, 0x80000001, 1, CPSR::FLAG_N | CPSR::FLAG_C);

    //   2147483649 - 2 = 2147483647            //
    //  -2147483647 - 2 = 2147483647  : --CV    //
    cnt += runTestSub(__LINE__, 0x80000001, 2, CPSR::FLAG_C | CPSR::FLAG_V);

    CPPUNIT_ASSERT_EQUAL( 0, cnt );
    return;
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
