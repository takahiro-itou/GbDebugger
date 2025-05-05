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
**      An Implementation of Test Case 'MemoryManager'.
**
**      @file       Common/Tests/MemoryManagerTest.cpp
**/

#include    "TestDriver.h"
#include    "GbDebugger/GbaMan/MemoryManager.h"

#include    "TestConf.h"


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

//========================================================================
//
//    MemoryManagerTest  class.
//
/**
**    クラス MemoryManager  の単体テスト。
**/

class  MemoryManagerTest : public  TestFixture
{
    CPPUNIT_TEST_SUITE(MemoryManagerTest);
    CPPUNIT_TEST(testCtor);
    CPPUNIT_TEST_SUITE_END();

public:
    virtual  void   setUp()     override    { }
    virtual  void   tearDown()  override    { }

private:
    void  testCtor();
};

CPPUNIT_TEST_SUITE_REGISTRATION( MemoryManagerTest );

//========================================================================
//
//    Tests.
//

void  MemoryManagerTest::testCtor()
{
    MemoryManager  testee;

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
