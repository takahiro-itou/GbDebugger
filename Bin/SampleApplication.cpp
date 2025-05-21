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

#include    "GbDebugger/GbaMan/BaseCpuCore.h"
#include    "GbDebugger/GbaMan/GbaManager.h"

#include    <iostream>
#include    <time.h>


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
    manGba.writeMnemonicCurrent(std::cout, manGba.getNextPC())
            <<  std::endl;

    //  最初の命令を実行。  //
    GbaMan::InstExecResult  ret = GbaMan::InstExecResult::SUCCESS_CONTINUE;

    int cnt = 0;
    clock_t clkSta  = clock();
    while ( ret != GbaMan::InstExecResult::UNDEFINED_OPECODE ) {
        ret = manGba.executeCurrentInst();

#if defined( _DEBUG )
        //  レジスタをダンプ。  //
        std::cout   <<  "REGS\n";
        manGba.printRegisters(std::cout)
                <<  std::endl;

        //  次の命令を逆アセンブル。    //
        std::cout   <<  "Mnemonic:\t"  <<  cnt  <<  "\n";
        manGba.writeMnemonicCurrent(std::cout, manGba.getNextPC())
                <<  std::endl;
#endif
        ++ cnt;
        if ( !(cnt & 0x07FFFFFF) ) {
            ClockCount  cc  = manGba.getCpuTotalTicks();
            clock_t clkEnd  = clock();
            const double elapsed = static_cast<double>(clkEnd - clkSta)
                    * 1000.0 / CLOCKS_PER_SEC;
            std::cout   <<  "Instructions: "
                        <<  cnt <<  ", "
                        <<  elapsed <<  "ms : "
                        <<  (cnt / elapsed)  <<  " kHz"
                        <<  std::endl;
            std::cout   <<  "Clock Counts: "
                        <<  cc  <<  ", "
                        <<  elapsed <<  "ms : "
                        <<  (cc / elapsed)  <<  " kHz"
                        <<  std::endl;
        }
    }

    ClockCount  cc  = manGba.getCpuTotalTicks();

    clock_t clkEnd  = clock();
    const double elapsed = static_cast<double>(clkEnd - clkSta)
                                * 1000.0 / CLOCKS_PER_SEC;
    std::cout   <<  "Instructions: "
                <<  cnt <<  ", "
                <<  elapsed <<  "ms : "
                <<  (cnt / elapsed)  <<  " kHz"
                <<  std::endl;
    std::cout   <<  "Clock Counts: "
                <<  cc  <<  ", "
                <<  elapsed <<  "ms : "
                <<  (cc / elapsed)  <<  " kHz"
                <<  std::endl;
    return ( 0 );
}
