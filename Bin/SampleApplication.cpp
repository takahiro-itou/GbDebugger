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
**      サンプルプログラム。
**
**      @file       Bin/SampleApplication.cpp
**/

#include    "GbDebugger/Common/SampleDocument.h"

#include    <iostream>

using   namespace   GBDEBUGGER_NAMESPACE;

int  main(int argc, char * argv[])
{
    Common::SampleDocument  test;
    std::string     input;

    std::cout   <<  "Input:";
    std::cin    >>  input;

    test.setMessage(input);
    std::cout   <<  "The number of alphabet in "
                <<  input
                <<  " = "
                <<  test.countAlphabet()
                <<  std::endl;
    return ( 0 );
}
