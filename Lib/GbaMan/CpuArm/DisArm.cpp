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

struct  MnemonicMap  {
    OpeCode         mask;
    OpeCode         cval;
    const char *    mnemonic;
};

const char * regNames[16] = {
    "R0" , "R1" , "R2" , "R3" , "R4" , "R5", "R6", "R7",
    "R8" , "R9" , "R10", "R11", "R12", "SP", "LR", "PC"
};

const char * conditions[16] = {
    "EQ", "NE", "CS", "CC", "MI", "PL", "VS", "VC",
    "HI", "LS", "GE", "LT", "GT", "LE", "AL", "NV"
};

CONSTEXPR_VAR   const  char  *  shiftTypes[5] = {
    "LSL", "LSR", "ASR", "ROR", "RRX",
};

CONSTEXPR_VAR   const  MnemonicMap
armMnemonics[] = {
    //  Branch
    { 0x0FF000F0, 0x01200010, "BX.%c\t%r00" },
    { 0x0F000000, 0x0A000000, "B.%c\t%o" },
    { 0x0F000000, 0x0B000000, "BL.%c\t%o" },
    { 0x0F000000, 0x0F000000, "SWI.%c\t%q" },

    //  LDR / STR   //
    { 0x0F700000, 0x04000000, "STR.%c \t%r12, [%r16], %ai" },
    { 0x0F700000, 0x04100000, "LDR.%c \t%r12, [%r16], %ai" },
    { 0x0F700000, 0x04200000, "STR.%c \t%r12, [%r16], %ai" },
    { 0x0F700000, 0x04300000, "LDR.%c \t%r12, [%r16], %ai" },
    { 0x0F700000, 0x04400000, "STR.%cB\t%r12, [%r16], %ai" },
    { 0x0F700000, 0x04500000, "LDR.%cB\t%r12, [%r16], %ai" },
    { 0x0F700000, 0x04600000, "STR.%cB\t%r12, [%r16], %ai" },
    { 0x0F700000, 0x04700000, "LDR.%cB\t%r12, [%r16], %ai" },
    { 0x0F700000, 0x05000000, "STR.%c \t%r12, [%r16, %ai]" },
    { 0x0F700000, 0x05100000, "LDR.%c \t%r12, [%r16, %ai]" },
    { 0x0F700000, 0x05200000, "STR.%c \t%r12, [%r16, %ai]!" },
    { 0x0F700000, 0x05300000, "LDR.%c \t%r12, [%r16, %ai]!" },
    { 0x0F700000, 0x05400000, "STR.%cB\t%r12, [%r16, %ai]" },
    { 0x0F700000, 0x05500000, "LDR.%cB\t%r12, [%r16, %ai]" },
    { 0x0F700000, 0x05600000, "STR.%cB\t%r12, [%r16, %ai]!" },
    { 0x0F700000, 0x05700000, "LDR.%cB\t%r12, [%r16, %ai]!" },

    { 0x0F700000, 0x06000000, "STR.%c \t%r12, [%r16], %Rs" },
    { 0x0F700000, 0x06100000, "LDR.%c \t%r12, [%r16], %Rs" },
    { 0x0F700000, 0x06200000, "STR.%c \t%r12, [%r16], %Rs" },
    { 0x0F700000, 0x06300000, "LDR.%c \t%r12, [%r16], %Rs" },
    { 0x0F700000, 0x06400000, "STR.%cB\t%r12, [%r16], %Rs" },
    { 0x0F700000, 0x06500000, "LDR.%cB\t%r12, [%r16], %Rs" },
    { 0x0F700000, 0x06600000, "STR.%cB\t%r12, [%r16], %Rs" },
    { 0x0F700000, 0x06700000, "LDR.%cB\t%r12, [%r16], %Rs" },
    { 0x0F700000, 0x07000000, "STR.%c \t%r12, [%r16, %Rs]" },
    { 0x0F700000, 0x07100000, "LDR.%c \t%r12, [%r16, %Rs]" },
    { 0x0F700000, 0x07200000, "STR.%c \t%r12, [%r16, %Rs]!" },
    { 0x0F700000, 0x07300000, "LDR.%c \t%r12, [%r16, %Rs]!" },
    { 0x0F700000, 0x07400000, "STR.%cB\t%r12, [%r16, %Rs]" },
    { 0x0F700000, 0x07500000, "LDR.%cB\t%r12, [%r16, %Rs]" },
    { 0x0F700000, 0x07600000, "STR.%cB\t%r12, [%r16, %Rs]!" },
    { 0x0F700000, 0x07700000, "LDR.%cB\t%r12, [%r16, %Rs]!" },

    //  ALU (Bit25==0 : 第二オペランドはレジスタ)   //
    { 0x0FF00000, 0x00000000, "AND.%c%s\t%r12, %16, %Rs" },
    { 0x0FF00000, 0x00200000, "EOR.%c%s\t%r12, %16, %Rs" },
    { 0x0FF00000, 0x00400000, "SUB.%c%s\t%r12, %16, %Rs" },
    { 0x0FF00000, 0x00600000, "RSB.%c%s\t%r12, %16, %Rs" },
    { 0x0FF00000, 0x00800000, "ADD.%c%s\t%r12, %16, %Rs" },
    { 0x0FF00000, 0x00A00000, "ADC.%c%s\t%r12, %16, %Rs" },
    { 0x0FF00000, 0x00C00000, "SBC.%c%s\t%r12, %16, %Rs" },
    { 0x0FF00000, 0x00E00000, "RSC.%c%s\t%r12, %16, %Rs" },
    { 0x0FF00000, 0x01000000, "TST.%c%s\t%%16, %Rs" },
    { 0x0FF00000, 0x01200000, "TEQ.%c%s\t%%16, %Rs" },
    { 0x0FF00000, 0x01400000, "CMP.%c%s\t%%16, %Rs" },
    { 0x0FF00000, 0x01600000, "CMN.%c%s\t%%16, %Rs" },
    { 0x0FF00000, 0x01800000, "ORR.%c%s\t%r12, %16, %Rs" },
    { 0x0FF00000, 0x01A00000, "MOV.%c%s\t%r12, %Rs" },
    { 0x0FF00000, 0x01C00000, "BIC.%c%s\t%r12, %16, %Rs" },
    { 0x0FF00000, 0x01E00000, "MVN.%c%s\t%r12, %Rs" },

    //  ALU (Bit25==0 : 第二オペランドはレジスタ)   //
    { 0x0FF00000, 0x02000000, "AND.%c%s\t%r12, %16, %i" },
    { 0x0FF00000, 0x02200000, "EOR.%c%s\t%r12, %16, %i" },
    { 0x0FF00000, 0x02400000, "SUB.%c%s\t%r12, %16, %i" },
    { 0x0FF00000, 0x02600000, "RSB.%c%s\t%r12, %16, %i" },
    { 0x0FF00000, 0x02800000, "ADD.%c%s\t%r12, %16, %i" },
    { 0x0FF00000, 0x02A00000, "ADC.%c%s\t%r12, %16, %i" },
    { 0x0FF00000, 0x02C00000, "SBC.%c%s\t%r12, %16, %i" },
    { 0x0FF00000, 0x02E00000, "RSC.%c%s\t%r12, %16, %i" },
    { 0x0FF00000, 0x03000000, "TST.%c%s\t%%16, %i" },
    { 0x0FF00000, 0x03200000, "TEQ.%c%s\t%%16, %i" },
    { 0x0FF00000, 0x03400000, "CMP.%c%s\t%%16, %i" },
    { 0x0FF00000, 0x03600000, "CMN.%c%s\t%%16, %i" },
    { 0x0FF00000, 0x03800000, "ORR.%c%s\t%r12, %16, %i" },
    { 0x0FF00000, 0x03A00000, "MOV.%c%s\t%r12, %i" },
    { 0x0FF00000, 0x03C00000, "BIC.%c%s\t%r12, %16, %i" },
    { 0x0FF00000, 0x03E00000, "MVN.%c%s\t%r12, %i" },

    //  Unknown
    { 0x00000000, 0x00000000, "[ ??? ]" },
};

//----------------------------------------------------------------
//  %ai - Addressing Immediate
//

inline  size_t
writeAddressingImmediate(
        const   OpeCode     opeCode,
        char  *  const      dst,
        const  char  *    & src,
        GuestMemoryAddress  gmAddr)
{
    const  OpeCode  ofs = (opeCode & 0x0FFF);
    const  char     sgn = (opeCode & 0x00800000) ? '+' : '-';
    return  sprintf(dst, "%c#0x%0x", sgn, ofs);
}

//----------------------------------------------------------------
//  %ar - Addressing Register

inline  size_t
writeOpe2RegisterWithShift(
        const   OpeCode     opeCode,
        char  *  const      dst,
        const  char  *    & src,
        GuestMemoryAddress  gmAddr);

inline  size_t
writeAddressingRegister(
        const   OpeCode     opeCode,
        char  *  const      dst,
        const  char  *    & src,
        GuestMemoryAddress  gmAddr)
{
    char    buf[512];

    const  OpeCode  reg = (opeCode & 0x0F);
    writeOpe2RegisterWithShift(opeCode, buf, src, gmAddr);
    return  sprintf(dst, "%s", regNames[reg]);
}

//----------------------------------------------------------------
//  %a? - Addressing

inline  size_t
writeAddressing(
        const   OpeCode     opeCode,
        char  *  const      dst,
        const  char  *    & src,
        GuestMemoryAddress  gmAddr)
{
    char    buf[256];

    const  OpeCode  reg = (opeCode >> 16) & 0x0F;
    const  char  fi = (*(++ src));

    switch ( fi ) {
    case  'i':
        return  writeAddressingImmediate(opeCode, dst, src, gmAddr);
        break;
    case  'r':
        return  writeAddressingRegister(opeCode, dst, src, gmAddr);
        break;
    }

    if ( !(opeCode & 0x01000000) ) {
        //  Bit 24 P=0 (Post)
        return  sprintf(dst, "[%s], %s", regNames[reg], buf);
    }
    //  Bit 24 P=1 (Pre)
    if ( (opeCode & 0x00200000) ) {
        //  Bit 20. Write Back. //
        return  sprintf(dst, "[%s, %s]!", regNames[reg], buf);
    }
    return  sprintf(dst, "[%s, %s]", regNames[reg], buf);
}

//----------------------------------------------------------------
//  %c - condition (bit 28-31)
//

inline  size_t
writeCondition(
        const   OpeCode     opeCode,
        char  *  const      dst,
        const  char  *    & src,
        GuestMemoryAddress  gmAddr)
{
    return  sprintf(dst, "%s", conditions[opeCode >> 28]);
}

//----------------------------------------------------------------
//  %o - offset (branch instruction)
//

inline  size_t
writeOffset(
        const   OpeCode     opeCode,
        char  *  const      dst,
        const  char  *    & src,
        GuestMemoryAddress  gmAddr)
{
    int ofs = opeCode & 0x00FFFFFF;
    if ( ofs & 0x00800000 ) {
        ofs |= 0xFF000000;
    }
    ofs <<= 2;
    return   sprintf(dst, "$%08x ; (%08x)", ofs, gmAddr + 8 + ofs);
}

//----------------------------------------------------------------
//  %rxx - Register.
//  xx : オペコードのどのビットからレジスタ番号を読みだすか
//

inline  size_t
writeRegister(
        const   OpeCode     opeCode,
        char  *  const      dst,
        const  char  *    & src,
        GuestMemoryAddress  gmAddr)
{
    const  int  reg_id0 = (*(++ src) - '0');
    const  int  reg_id1 = (*(++ src) - '0');
    const  int  reg_bit = ((reg_id0 * 10) + reg_id1);
    const  int  reg_id  = (opeCode >> reg_bit) & 0x0F;
    return  sprintf(dst, "%s", regNames[reg_id]);
}

//----------------------------------------------------------------
//  %i - シフトされた即値 (ALU)

inline  size_t
writeOp2ImmWithRor(
        const   OpeCode     opeCode,
        char  *  const      dst,
        const  char  *    & src,
        GuestMemoryAddress  gmAddr)
{
    const  RegType  imm = (opeCode & 0xFF);
    const  int      rot = (opeCode & 0xF00) >> 7;
    const  RegType  val = armRorVal(imm, rot);
    return  sprintf(dst, "#0x%08x", val);
}

//----------------------------------------------------------------
//  %Rs - シフトされたレジスタ (ALU)
//

inline  size_t
writeOpe2RegisterWithShift(
        const   OpeCode     opeCode,
        char  *  const      dst,
        const  char  *    & src,
        GuestMemoryAddress  gmAddr)
{
    int st  = (opeCode >> 5) & 0x03;    //  シフトタイプ。  //
    int sw  = (opeCode >> 7) & 0x1F;    //  シフト幅。      //
    if ( opeCode & 0x10 ) {
        return  sprintf(dst, "%s, %s %s",
                        regNames[opeCode & 0x0F],
                        shiftTypes[st],
                        regNames[(opeCode >> 8) & 0x0F]);
    }
    if ( (sw == 0) && (st == 3) ) {
        st  = 4;
    }
    if ( sw == 0 ) {
        if ( st == 3 ) {
            st  = 4;
            sw  = 1;
        } else if ( (st == 1) || (st == 2) ) {
            sw  = 32;
        }
    }
    if ( (sw) || (st) ) {
        return  sprintf(dst, "%s, %s #0x%08x",
                        regNames[opeCode & 0x0F],
                        shiftTypes[st],
                        sw);
    }
    return  sprintf(dst, "%s", regNames[opeCode % 0x0F]);
}

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

    const MnemonicMap *  oc = armMnemonics;
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
            case  'R':
                if ( *(++ src) == 's' ) {
                    len = writeOpe2RegisterWithShift(opeCode, dst, src, gmAddr);
                }
                break;
            case  'a':
                len = writeAddressing(opeCode, dst, src, gmAddr);
                break;
            case  'c':
                //  len = sprintf(dst, "%s", conditions[opeCode >> 28]);
                len = writeCondition(opeCode, dst, src, gmAddr);
                break;
            case  'i':
                len = writeOp2ImmWithRor(opeCode, dst, src, gmAddr);
                break;
            case  'r':
                // reg_id  = (opeCode >> ((*(++ src) - '0') * 4)) & 15;
                // len = sprintf(dst, "%s", regNames[reg_id]);
                len = writeRegister(opeCode, dst, src, gmAddr);
                break;
            case  'o':
                len = writeOffset(opeCode, dst, src, gmAddr);
                break;
            case  's':
                if ( opeCode & 0x00100000 ) {
                    *(dst ++)   = 's';
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
