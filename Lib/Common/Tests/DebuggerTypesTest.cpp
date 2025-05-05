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
**      An Implementation of Test Case 'DebuggerTypes'.
**
**      @file       Common/Tests/DebuggerTypesTest.cpp
**/

#include    "TestDriver.h"
#include    "GbDebugger/Common/DebuggerTypes.h"


GBDEBUGGER_NAMESPACE_BEGIN

//========================================================================
//
//    DebuggerTypesTest  class.
//
/**
**    クラス DebuggerTypes の単体テスト。
**/

class  DebuggerTypesTest : public  TestFixture
{
    CPPUNIT_TEST_SUITE(DebuggerTypesTest);
    CPPUNIT_TEST(testErrCode);
    CPPUNIT_TEST(testPointerCast);
    CPPUNIT_TEST_SUITE_END();

public:
    virtual  void   setUp()     override    { }
    virtual  void   tearDown()  override    { }

private:
    void  testErrCode();
    void  testPointerCast();
};

CPPUNIT_TEST_SUITE_REGISTRATION( DebuggerTypesTest );

//========================================================================
//
//    Tests.
//

void  DebuggerTypesTest::testErrCode()
{
    ErrCode retCode = ErrCode::SUCCESS;

    CPPUNIT_ASSERT_EQUAL( ErrCode::SUCCESS, retCode );
    CPPUNIT_ASSERT_EQUAL( 0, static_cast<int>(retCode) );

    retCode = static_cast<ErrCode>(1);
    CPPUNIT_ASSERT_EQUAL( ErrCode::FAILURE, retCode );

    return;
}

void  DebuggerTypesTest::testPointerCast()
{
    int tmp = 0x12345678;
    int8_t  *   ptr = pointer_cast<int8_t *>(&tmp);
    (* ptr) = -1;

    CPPUNIT_ASSERT_EQUAL(0x123456FF, tmp);

    return;
}

GBDEBUGGER_NAMESPACE_END

//========================================================================
//
//    エントリポイント。
//

int  main(int argc, char * argv[])
{
    return ( executeCppUnitTests(argc, argv) );
}
