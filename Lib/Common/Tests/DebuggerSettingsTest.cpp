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
**      An Implementation of Test Case 'DebuggerSettings'.
**
**      @file       Common/Tests/DebuggerSettingsTest.cpp
**/

#include    "TestDriver.h"
#include    "GbDebugger/Common/DebuggerSettings.h"


GBDEBUGGER_NAMESPACE_BEGIN

//========================================================================
//
//    DebuggerSettingsTest  class.
//
/**
**    クラス DebuggerSettings の単体テスト。
**/

class  DebuggerSettingsTest : public  TestFixture
{
    CPPUNIT_TEST_SUITE(DebuggerSettingsTest);
    CPPUNIT_TEST(testNameSpace);
    CPPUNIT_TEST_SUITE_END();

public:
    virtual  void   setUp()     override    { }
    virtual  void   tearDown()  override    { }

private:
    void  testNameSpace();
};

CPPUNIT_TEST_SUITE_REGISTRATION( DebuggerSettingsTest );

//========================================================================
//
//    Tests.
//

void  DebuggerSettingsTest::testNameSpace()
{
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
