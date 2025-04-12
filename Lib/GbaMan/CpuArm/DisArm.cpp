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
**      An Implementation of DisArm class.
**
**      @file       GbaMan/DisArm.cpp
**/

#include    "DisArm.h"
#include    "GbDebugger/GbaMan/GbaManager.h"

#include    <ostream>


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

namespace  {

struct  Opecodes  {
    uint32_t    mask;
    uint32_t    cval;
    const char *    mnemonic;
};

const char * regs[16] = {
    "R0" , "R1" , "R2" , "R3" , "R4" , "R5", "R6", "R7",
    "R8" , "R9" , "R10", "R11", "R12", "SP", "LR", "PC"
};

const char * conditions[16] = {
    "EQ", "NE", "CS", "CC", "MI", "PL", "VS", "VC",
    "HI", "LS", "GE", "LT", "GT", "LE", "AL", "NV"
};

const Opecodes armOpecodes[] = {
    //  Branch
    { 0x0FF000F0, 0x01200010, "BX.%c\t%r0" },
    { 0x0F000000, 0x0A000000, "B.%c\t%o" },
    { 0x0F000000, 0x0B000000, "BL.%c\t%o" },
    { 0x0F000000, 0x0F000000, "SWI.%c\t%q" },
    { 0x00000000, 0x00000000, "[ ??? ]" }
};

}   //  End of (Unnamed) namespace.


//========================================================================
//
//    DisArm  class.
//

//========================================================================
//
//    Constructor(s) and Destructor.
//

//----------------------------------------------------------------
//    インスタンスを初期化する
//  （デフォルトコンストラクタ）。
//

DisArm::DisArm()
{
}

//----------------------------------------------------------------
//    インスタンスを破棄する
//  （デストラクタ）。
//

DisArm::~DisArm()
{
}

//========================================================================
//
//    Public Member Functions (Implement Pure Virtual).
//

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

//----------------------------------------------------------------
//    ニーモニックを表示する。
//

std::ostream  &
DisArm::writeMnemonic(
        std::ostream       &outStr,
        GuestMemoryAddress  gmAddr,
        const  OpeCode      opeCode)  const
{
    char    buf[256] = { 0 };

    const Opecodes * oc = armOpecodes;
    for ( ; (opeCode & oc->mask) != oc->cval; ++ oc ) ;

    sprintf(buf, "%08x:   %08x\t", gmAddr, opeCode);
    outStr  <<  buf;

    int             reg_id  = 0;
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
            case  'c':
                len = sprintf(dst, "%s", conditions[opeCode >> 28]);
                break;
            case  'r':
                reg_id  = (opeCode >> ((*(++ src) - '0') * 4)) & 15;
                len = sprintf(dst, "%s", regs[reg_id]);
                break;
            case  'o':  {
                * (dst ++)  = '$';
                int off = opeCode & 0x00FFFFFF;
                if ( off & 0x00800000 ) {
                    off |= 0xFF000000;
                }
                off <<= 2;
                len = sprintf(dst, "%08x ; (%08x)", off, gmAddr + 8 + off);
                }
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
