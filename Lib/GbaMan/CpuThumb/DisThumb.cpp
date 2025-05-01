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
**      An Implementation of DisThumb class.
**
**      @file       GbaMan/DisThumb.cpp
**/

#include    "DisThumb.h"


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

namespace  {

CONSTEXPR_VAR   const  MnemonicMap
thumbMnemonics[] = {
    //  Format 01 : ビットシフト。  //
    { 0xF800, 0x0000, "LSL" },
    { 0xF800, 0x0800, "LSR" },
    { 0xF800, 0x1000, "ASR" },

    //  Format 02 : 加算減算。      //
    { 0xFE00, 0x1800, "ADD" },
    { 0xFE00, 0x1A00, "SUB" },
    { 0xFE00, 0x1C00, "ADD" },
    { 0xFE00, 0x1E00, "SUB" },

    //  Format 03 : 即値の演算。    //
    { 0xF800, 0x2000, "MOV" },
    { 0xF800, 0x2800, "CMP" },
    { 0xF800, 0x3000, "ADD" },
    { 0xF800, 0x3800, "SUB" },

    //  Format 04 : 算術演算。      //
    { 0xFFC0, 0x4000, "AND" },
    { 0xFFC0, 0x4040, "EOR" },
    { 0xFFC0, 0x4080, "LSL" },
    { 0xFFC0, 0x40C0, "LSR" },
    { 0xFFC0, 0x4100, "ASR" },
    { 0xFFC0, 0x4140, "ADC" },
    { 0xFFC0, 0x4180, "SBC" },
    { 0xFFC0, 0x41C0, "ROR" },
    { 0xFFC0, 0x4200, "TST" },
    { 0xFFC0, 0x4240, "NEG" },
    { 0xFFC0, 0x4280, "CMP" },
    { 0xFFC0, 0x42C0, "CMN" },
    { 0xFFC0, 0x4300, "ORR" },
    { 0xFFC0, 0x4340, "MUL" },
    { 0xFFC0, 0x4380, "BIC" },
    { 0xFFC0, 0x43C0, "MVN" },

    //  Format 05 : R8-15 レジスタ操作。    //
    { 0xFCC0, 0x4400, "[ ??? ]" },      //  MSBd==0 && MSBs==0 は不正。 //
    { 0xFF00, 0x4400, "ADD" },
    { 0xFF00, 0x4500, "CMP" },
    { 0xFF00, 0x4600, "MOV" },
    { 0xFF80, 0x4700, "BX"  },          //  MSBd は 0。Rd は未使用。    //

    //  Format 06 : ロードストア命令（PC-Relative）。   //
    { 0xF800, 0x4800, "LDR" },

    //  Format 07 : ロードストア命令。  //
    { 0xFE00, 0x5000, "STR" },
    { 0xFE00, 0x5400, "STRB" },
    { 0xFE00, 0x5800, "LDR" },
    { 0xFE00, 0x5C00, "LDRB" },

    //  Format 08: ロードストア命令。   //
    { 0xFE00, 0x5200, "STRH" },
    { 0xFE00, 0x5600, "LDSB" },
    { 0xFE00, 0x5A00, "LDRH" },
    { 0xFE00, 0x5E00, "LDSH" },

    //  Format 09 : ロードストア命令。  //
    { 0xF800, 0x6000, "STR" },
    { 0xF800, 0x6800, "LDR" },
    { 0xF800, 0x7000, "STRB" },
    { 0xF800, 0x7800, "LDRB" },

    //  Format 10: ロードストア命令（ハーフワード）。   //
    { 0xF800, 0x8000, "STRH" },
    { 0xF800, 0x8800, "LDRH" },

    //  Format 11 : ロードストア命令（SP-Relative）。   //
    { 0xF800, 0x9000, "STR Rd, [SP, #nn]" },
    { 0xF800, 0x9800, "LDR Rd, [SP, #nn]" },

    //  Format 12: アドレッシング。 //
    { 0xF800, 0xA000, "ADD Rd, PC, #nn" },
    { 0xF800, 0xA800, "ADD Rd, SP, #nn" },

    //  Format 13: SP操作。 //
    { 0xFF80, 0xB000, "ADD SP, #nn" },
    { 0xFF80, 0xB080, "ADD SP, #-nn" },

    //  Unknown.    //
    { 0x0000, 0x0000, "[ ??? ]" },
};

}   //  End of (Unnamed) namespace.


//========================================================================
//
//    DisThumb  class.
//

//========================================================================
//
//    Constructor(s) and Destructor.
//

//----------------------------------------------------------------
//    インスタンスを初期化する
//  （デフォルトコンストラクタ）。
//

DisThumb::DisThumb()
{
}

//----------------------------------------------------------------
//    インスタンスを破棄する
//  （デストラクタ）。
//

DisThumb::~DisThumb()
{
}

//========================================================================
//
//    Public Member Functions (Implement Pure Virtual).
//

//----------------------------------------------------------------
//    ニーモニックを表示する。
//

std::ostream  &
DisThumb::writeMnemonic(
        std::ostream       &outStr,
        GuestMemoryAddress  gmAddr,
        const  OpeCode      opeCode)  const
{
    char    buf[256] = { 0 };

    const MnemonicMap *  oc = thumbMnemonics;
    for ( ; (opeCode & oc->mask) != oc->cval; ++ oc ) ;

    sprintf(buf, "%08x:   %08x\t", gmAddr, opeCode);
    outStr  <<  buf;

    size_t          len = 0;
    const  char  *  src = oc->mnemonic;
    char  *         dst = buf;

    while (*src) {
        len = 0;
        if ( *src != '%' ) {
            *(dst ++)   = *(src ++);
        } else {
            ++  src;
            switch ( *src ) {
            default:
                *(dst ++)   = '%';
                *(dst ++)   = *(src);
                break;
            }
            ++  src;
        }
        dst += len;
    }

    *(dst ++)   = '\0';
    outStr  <<  buf;

    return ( outStr );
}

//========================================================================
//
//    Public Member Functions (Overrides).
//

//========================================================================
//
//    Public Member Functions (Pure Virtual Functions).
//

//========================================================================
//
//    Public Member Functions (Virtual Functions).
//

//========================================================================
//
//    Public Member Functions.
//

//========================================================================
//
//    Accessors.
//

//========================================================================
//
//    Protected Member Functions.
//

//========================================================================
//
//    For Internal Use Only.
//

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END
