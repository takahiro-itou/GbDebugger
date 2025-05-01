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
    outStr  <<  "disassembleThumb Not Implemented." <<  std::endl;
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
