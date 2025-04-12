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

#include    "GbDebugger/GbaMan/GbaManager.h"

#include    <iostream>

using   namespace   GBDEBUGGER_NAMESPACE;

int  main(int argc, char * argv[])
{
    ErrCode retCode = ErrCode::SUCCESS;
    GbaMan::GbaManager  manGba;

    if ( argc < 2 ) {
        std::cerr   <<  "Usage "
                    <<  argv[0]
                    <<  " [rom file]"
                    <<  std::endl;
        return ( 1 );
    }

    if ( (retCode = manGba.openRomFile(argv[1])) != ErrCode::SUCCESS ) {
        std::cerr   <<  "ERROR : Open ROM "
                    <<  argv[1] <<  std::endl;
        return ( 1 );
    }

    //  ハードリセットを行う。      //
    manGba.doHardReset();

    //  最初のレジスタをダンプ。    //
    std::cout   <<  "REGS\n";
    manGba.printRegisters(std::cout)
            <<  std::endl;

    //  最初の命令を逆アセンブル。  //
    std::cout   <<  "Mnemonic:\n";
    manGba.disassembleArm(std::cout, manGba.getNextPC())
            <<  std::endl;

    //  最初の命令を実行。  //
    manGba.executeCurrentInst();

    //  レジスタをダンプ。  //
    std::cout   <<  "REGS\n";
    manGba.printRegisters(std::cout)
            <<  std::endl;

    //  次の命令を逆アセンブル。    //
    std::cout   <<  "Mnemonic:\n";
    manGba.disassembleArm(std::cout, manGba.getNextPC())
            <<  std::endl;

        return ( 0 );
}
