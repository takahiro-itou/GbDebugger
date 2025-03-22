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
**      An Implementation of Test Case 'GbaManager'.
**
**      @file       Common/Tests/GbaManagerTest.cpp
**/

#include    "TestDriver.h"
#include    "GbDebugger/GbaMan/GbaManager.h"


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

//========================================================================
//
//    GbaManagerTest  class.
//
/**
**    クラス GbaManager の単体テスト。
**/

class  GbaManagerTest : public  TestFixture
{
    CPPUNIT_TEST_SUITE(GbaManagerTest);
    CPPUNIT_TEST(testCtor);
    CPPUNIT_TEST(testOpenRomFile);
    CPPUNIT_TEST_SUITE_END();

public:
    virtual  void   setUp()     override    { }
    virtual  void   tearDown()  override    { }

private:
    void  testCtor();
    void  testOpenRomFile();
};

CPPUNIT_TEST_SUITE_REGISTRATION( GbaManagerTest );

//========================================================================
//
//    Tests.
//

void  GbaManagerTest::testCtor()
{
    GbaManager  testee;

    return;
}

void  GbaManagerTest::testOpenRomFile()
{
    ErrCode     retCode;
    GbaManager  testee;

    retCode = testee.openRomFile("no-such-file");
    CPPUNIT_ASSERT_EQUAL( ErrCode::FILE_OPEN_ERROR, retCode );

    retCode = testee.openRomFile("/dev/null");
    CPPUNIT_ASSERT_EQUAL( ErrCode::FILE_IO_ERROR, retCode );

    retCode = testee.openRomFile("badrom.gba");
    CPPUNIT_ASSERT_EQUAL( ErrCode::FILE_INVALID_FORMAT, retCode );

    retCode = testee.openRomFile("hello.gba");
    CPPUNIT_ASSERT_EQUAL( ErrCode::SUCCESS, retCode );

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
