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

#include    "GbDebugger/GbaMan/MemoryManager.h"


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

namespace  {

CONSTEXPR_VAR   const  MnemonicMap
thumbMnemonics[] = {
    //  Format 01 : ビットシフト。  //
    { 0xF800, 0x0000, "LSL \t%r0, %r3, #%I{6,31}" },
    { 0xF800, 0x0800, "LSR \t%r0, %r3, #%I{6,31}" },
    { 0xF800, 0x1000, "ASR \t%r0, %r3, #%I{6,31}" },

    //  Format 02 : 加算減算。      //
    { 0xFE00, 0x1800, "ADD" },
    { 0xFE00, 0x1A00, "SUB" },
    { 0xFE00, 0x1C00, "ADD" },
    { 0xFE00, 0x1E00, "SUB" },

    //  Format 03 : 即値の演算。    //
    { 0xF800, 0x2000, "MOV \t%r8, #%I{0,255}" },
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
    { 0xFF00, 0x4400, "ADD \t%m0+7, %m3+6" },
    { 0xFF00, 0x4500, "CMP \t%m0+7, %m3+6" },
    { 0xFF00, 0x4600, "MOV \t%m0+7, %m3+6" },
    { 0xFF80, 0x4700, "BX  \t%m0+7, %m3+6" },   //  MSBd は 0。Rd は未使用  //

    //  Format 06 : ロードストア命令（PC-Relative）。   //
    { 0xF800, 0x4800, "LDR\t%r8, [PC, #%n2]\t; %P2" },

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

    //  Format 12 : アドレッシング。    //
    { 0xF800, 0xA000, "ADD Rd, PC, #nn" },
    { 0xF800, 0xA800, "ADD Rd, SP, #nn" },

    //  Format 13 : SP操作。    //
    { 0xFF80, 0xB000, "ADD SP, #nn" },
    { 0xFF80, 0xB080, "ADD SP, #-nn" },

    //  Format 14 : ロードストア命令（スタック）。  //
    { 0xFFFF, 0xB500, "PUSH {LR}" },
    { 0xFF00, 0xB400, "PUSH {Rlist}" },
    { 0xFF00, 0xB500, "PUSH {Rlist,LR}" },
    { 0xFFFF, 0xBD00, "POP {PC}" },
    { 0xFF00, 0xBC00, "POP {Rlist}" },
    { 0xFF00, 0xBD00, "POP {Rlist,PC}" },

    //  Format 15   //
    { 0xF800, 0xC000, "STMIA Rb!, {Rlist}" },
    { 0xF800, 0xC800, "LDMIA Rb!, {Rlist}" },

    //  Format 16 : 条件付き分岐命令。      //
    //  Format 17 : ソフトウェア割り込み。  //
    { 0xFF00, 0xD000, "BEQ\t%os1" },
    { 0xFF00, 0xD100, "BNE\t%os1" },
    { 0xFF00, 0xD200, "BCS\t%os1" },
    { 0xFF00, 0xD300, "BCC\t%os1" },
    { 0xFF00, 0xD400, "BMI\t%os1" },
    { 0xFF00, 0xD500, "BPL\t%os1" },
    { 0xFF00, 0xD600, "BVS\t%os1" },
    { 0xFF00, 0xD700, "BVC\t%os1" },
    { 0xFF00, 0xD800, "BHI\t%os1" },
    { 0xFF00, 0xD900, "BLS\t%os1" },
    { 0xFF00, 0xDA00, "BGE\t%os1" },
    { 0xFF00, 0xDB00, "BLT\t%os1" },
    { 0xFF00, 0xDC00, "BGT\t%os1" },
    { 0xFF00, 0xDD00, "BLE\t%os1" },
    { 0xFF00, 0xDF00, "SWI" },
    { 0xFF00, 0xBE00, "BKPT" },

    //  Format 18 : 無条件分岐命令。    //
    { 0xF800, 0xE000, "B" },

    //  Format 19 : サブルーチンコール。    //
    { 0xF800, 0xF000, "BL" },
    { 0xF800, 0xF800, "BLH" },
    { 0xF800, 0xE800, "BLX" },

    //  Unknown.    //
    { 0x0000, 0x0000, "[ ??? ]" },
};

//----------------------------------------------------------------
//    相対オフセット。
//

inline  GuestMemoryAddress
getUnsignedOffset(
        const   OpeCode     opeCode,
        const   char  *    & src)
{
    const  int  sftBits = readMnemonicParameter(src, 1);
    return  static_cast<GuestMemoryAddress>((opeCode & 0x00FF) << sftBits);
}


//----------------------------------------------------------------
//  %P  - PC-Relative.
//

inline  size_t
writePCRelative(
        const   OpeCode             opeCode,
        char  *  const              dst,
        const   char  *           & src,
        const   MemoryManager     & manMem,
        const   GuestMemoryAddress  gmAddr)
{
    const   GuestMemoryAddress  nn  = getUnsignedOffset(opeCode, src);
    const   GuestMemoryAddress  pos = (gmAddr & ~3) + 4 + nn;

    //  読みだすアドレスが決定的、かつ大抵ロム上。  //
    //  なので値も定数だろうから読みだしておく。    //
    const  RegType  val = manMem.readMemory<RegType>(pos);
    return  sprintf(dst, "[%08x] (=$%08x)", pos, val);
}

//----------------------------------------------------------------
//  %I{bit,msk} - Immediate.
//  bit : オペコードのどのビットから即値を読みだすか
//  msk : 読みだした即値に掛けるマスク
//

inline  size_t
writeImmediate(
        const   OpeCode     opeCode,
        char  *  const      dst,
        const  char  *    & src,
        GuestMemoryAddress  gmAddr)
{
    RegType val = 0;
    int     dig = 4;

    if ( *(src) == '{' ) {
        ++  src;
        const  int  immBit  = readMnemonicParameter(src, 2);
        ++  src;    //  カンマを読み捨て。      //
        const  int  immMask = readMnemonicParameter(src, 8);
        ++  src;    //  末尾の }  を読み捨て。  //
        val = (opeCode >> immBit) & immMask;
    }

    return  sprintf(dst, "0x%0*x", dig, val);
}

//----------------------------------------------------------------
//  %rx - Register.
//  x : オペコードのどのビットからレジスタ番号を読みだすか
//

inline  size_t
writeRegister(
        const   OpeCode     opeCode,
        char  *  const      dst,
        const  char  *    & src,
        GuestMemoryAddress  gmAddr)
{
    const  int  regBit  = readMnemonicParameter(src, 1);
    const  int  regIdx  = (opeCode >> regBit) & 0x07;
    return  sprintf(dst, "%s", regNames[regIdx]);
}

//----------------------------------------------------------------
//  %m - Register With MSB
//

inline  size_t
writeRegisterHigh(
        const   OpeCode     opeCode,
        char  *  const      dst,
        const  char  *    & src,
        GuestMemoryAddress  gmAddr)
{
    const  int  regBit  = readMnemonicParameter(src, 1);
    ++  src;    //  プラス記号を読み捨て。
    const  int  msbBit  = readMnemonicParameter(src, 1);
    const  int  regIdx  = (((opeCode >> msbBit) << 3) & 0x0F)
                                | ((opeCode >> regBit) & 0x07);
    return  sprintf(dst, "%s", regNames[regIdx]);
}

//----------------------------------------------------------------
//  %ou/%os - オフセット。
//

inline  size_t
writeOffset(
        const   OpeCode     opeCode,
        char  *  const      dst,
        const  char  *    & src,
        GuestMemoryAddress  gmAddr)
{
    const   char        ch  = (*(src ++));
    GuestMemoryAddress  ofs = getUnsignedOffset(opeCode, src);

    if ( (ch == 's') && (ofs & 0x0080) ) {
        ofs |= 0xFFFFFF00;
    }

    return  sprintf(dst, "$%08x ; (%08x)", ofs, gmAddr + 4 + ofs);
}

//----------------------------------------------------------------
//  %nxx - 符号なしオフセット。
//

inline  size_t
writeUnsignedOffset(
        const   OpeCode     opeCode,
        char  *  const      dst,
        const  char  *    & src,
        GuestMemoryAddress  gmAddr)
{
    const   GuestMemoryAddress  nn  = getUnsignedOffset(opeCode, src);
    int     dig = 4;
    return  sprintf(dst, "0x%0*x", dig, nn);
}

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

    sprintf(buf, "%08x:   %04x\t", gmAddr, (opeCode & 0xFFFF));
    outStr  <<  buf;

    size_t          len = 0;
    const  char  *  src = oc->mnemonic;
    char  *         dst = buf;
    char            ch;

    while ( ch = *(src ++) ) {
        len = 0;
        if ( ch != '%' ) {
            * (dst ++)  = ch;
        } else {
            ch  = *(src ++);
            switch ( ch ) {
            case  'I':
                len = writeImmediate(opeCode, dst, src, gmAddr);
                break;
            case  'P':
                len = writePCRelative(
                            opeCode, dst, src, *(this->m_pManMem), gmAddr);
                break;
            case  'm':
                len = writeRegisterHigh(opeCode, dst, src, gmAddr);
                break;
            case  'n':
                len = writeUnsignedOffset(opeCode, dst, src, gmAddr);
                break;
            case  'o':
                len = writeOffset(opeCode, dst, src, gmAddr);
                break;
            case  'r':
                len = writeRegister(opeCode, dst, src, gmAddr);
                break;
            default:
                * (dst ++)  = '%';
                * (dst ++)  = ch;
                break;
            }
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
