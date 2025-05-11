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

#include    "../Utils/DisUtils.inl"

#include    <sstream>


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

namespace  {

CONSTEXPR_VAR   const  MnemonicMap
thumbMnemonics[] = {
    //  Format 01 : ビットシフト。  //
    { 0xF800, 0x0000, "LSL \t%r0, %r3, #%I{6,31,0}" },
    { 0xF800, 0x0800, "LSR \t%r0, %r3, #%I{6,31,0}" },
    { 0xF800, 0x1000, "ASR \t%r0, %r3, #%I{6,31,0}" },

    //  Format 02 : 加算減算。      //
    { 0xFE00, 0x1800, "ADD \t%r0, %r3, %r6" },
    { 0xFE00, 0x1A00, "SUB \t%r0, %r3, %r6" },
    { 0xFE00, 0x1C00, "ADD \t%r0, %r3, #%I{6,7,0}" },
    { 0xFE00, 0x1E00, "SUB \t%r0, %r3, #%I{6,7,0}" },

    //  Format 03 : 即値の演算。    //
    { 0xF800, 0x2000, "MOV \t%r8, #%I{0,255,0}" },
    { 0xF800, 0x2800, "CMP \t%r8, #%I{0,255,0}" },
    { 0xF800, 0x3000, "ADD \t%r8, #%I{0,255,0}" },
    { 0xF800, 0x3800, "SUB \t%r8, #%I{0,255,0}" },

    //  Format 04 : 算術演算。      //
    { 0xFFC0, 0x4000, "AND \t%r0, %r3" },
    { 0xFFC0, 0x4040, "EOR \t%r0, %r3" },
    { 0xFFC0, 0x4080, "LSL \t%r0, %r3" },
    { 0xFFC0, 0x40C0, "LSR \t%r0, %r3" },
    { 0xFFC0, 0x4100, "ASR \t%r0, %r3" },
    { 0xFFC0, 0x4140, "ADC \t%r0, %r3" },
    { 0xFFC0, 0x4180, "SBC \t%r0, %r3" },
    { 0xFFC0, 0x41C0, "ROR \t%r0, %r3" },
    { 0xFFC0, 0x4200, "TST \t%r0, %r3" },
    { 0xFFC0, 0x4240, "NEG \t%r0, %r3" },
    { 0xFFC0, 0x4280, "CMP \t%r0, %r3" },
    { 0xFFC0, 0x42C0, "CMN \t%r0, %r3" },
    { 0xFFC0, 0x4300, "ORR \t%r0, %r3" },
    { 0xFFC0, 0x4340, "MUL \t%r0, %r3" },
    { 0xFFC0, 0x4380, "BIC \t%r0, %r3" },
    { 0xFFC0, 0x43C0, "MVN \t%r0, %r3" },

    //  Format 05 : R8-15 レジスタ操作。    //
    { 0xFFC0, 0x4400, "[ ??? (R8-15) ]" },  //  MSBd==0 && MSBs==0 は不正。 //
    { 0xFFC0, 0x4500, "[ ??? (R8-15) ]" },  //  MSBd==0 && MSBs==0 は不正。 //
    { 0xFFC0, 0x4600, "[ ??? (R8-15) ]" },  //  MSBd==0 && MSBs==0 は不正。 //
    { 0xFF00, 0x4400, "ADD \t%m0+7, %m3+6" },
    { 0xFF00, 0x4500, "CMP \t%m0+7, %m3+6" },
    { 0xFF00, 0x4600, "MOV \t%m0+7, %m3+6" },
    { 0xFF80, 0x4780, "[ ??? (BX) ]" },         //  MSBd は 0。Rd は未使用  //
    { 0xFF80, 0x4700, "BX  \t%m3+6" },          //  MSBd は 0。Rd は未使用  //

    //  Format 06 : ロードストア命令（PC-Relative）。   //
    { 0xF800, 0x4800, "LDR \t%r8, [PC, #%I{0,255,2}]\t; %P2" },

    //  Format 07 : ロードストア命令。  //
    //  Format 08: ロードストア命令。   //
    { 0xFE00, 0x5000, "STR \t%r0, [%r3, %r6]" },
    { 0xFE00, 0x5400, "STRB\t%r0, [%r3, %r6]" },
    { 0xFE00, 0x5800, "LDR \t%r0, [%r3, %r6]" },
    { 0xFE00, 0x5C00, "LDRB\t%r0, [%r3, %r6]" },

    { 0xFE00, 0x5200, "STRH\t%r0, [%r3, %r6]" },
    { 0xFE00, 0x5600, "LDSB\t%r0, [%r3, %r6]" },
    { 0xFE00, 0x5A00, "LDRH\t%r0, [%r3, %r6]" },
    { 0xFE00, 0x5E00, "LDSH\t%r0, [%r3, %r6]" },

    //  Format 09 : ロードストア命令。  //
    { 0xF800, 0x6000, "STR \t%r0, [%r3, #%I{6,31,2}" },
    { 0xF800, 0x6800, "LDR \t%r0, [%r3, #%I{6,31,2}" },
    { 0xF800, 0x7000, "STRB\t%r0, [%r3, #%I{6,31,0}" },
    { 0xF800, 0x7800, "LDRB\t%r0, [%r3, #%I{6,31,0}" },

    //  Format 10: ロードストア命令（ハーフワード）。   //
    { 0xF800, 0x8000, "STRH\t%r0, [%r3, #%I{6,31,1}" },
    { 0xF800, 0x8800, "LDRH\t%r0, [%r3, #%I{6,31,1}" },

    //  Format 11 : ロードストア命令（SP-Relative）。   //
    { 0xF800, 0x9000, "STR \t%r8, [SP, #%I{0,255,2}]" },
    { 0xF800, 0x9800, "LDR \r%r8, [SP, #%I{0,255,2}]" },

    //  Format 12 : アドレッシング。    //
    { 0xF800, 0xA000, "ADD \t%r8, PC, #%I{0,255,2}" },
    { 0xF800, 0xA800, "ADD \t%r8, SP, #%I{0,255,2}" },

    //  Format 13 : SP操作。    //
    { 0xFF80, 0xB000, "ADD \tSP, #%I{0,127,2}" },
    { 0xFF80, 0xB080, "ADD \tSP, #-%I{0,127,2}" },

    //  Format 14 : ロードストア命令（スタック）。  //
    { 0xFFFF, 0xB500, "PUSH\t{LR}" },
    { 0xFF00, 0xB400, "PUSH\t{%l}" },
    { 0xFF00, 0xB500, "PUSH\t{%l,LR}" },
    { 0xFFFF, 0xBD00, "POP \t{PC}" },
    { 0xFF00, 0xBC00, "POP \t{$l}" },
    { 0xFF00, 0xBD00, "POP \t{%l,PC}" },

    //  Format 15   //
    { 0xF800, 0xC000, "STMIA\t%r8!, {%l}" },
    { 0xF800, 0xC800, "LDMIA\t%r8!, {%l}" },

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
    { 0xF800, 0xE000, "B   \t%p{0,2047,1}" },

    //  Format 19 : サブルーチンコール。    //
    { 0xF800, 0xF000, "BL  \t%L" },
    { 0xF800, 0xF800, "BLH \t%Z" },
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
//  %L  - Long (22bit) offset
//

inline  size_t
writeLongOffset(
        const   OpeCode             opeCode,
        const   OpeCode             nextOpe,
        char  *  const              dst,
        const   char  *           & src,
        const   GuestMemoryAddress  gmAddr)
{
    int32_t ofs = (opeCode & 0x07FF);
    if ( ofs & 0x0400 ) {
        ofs |= 0xFFFFF800;
    }
    ofs = (ofs << 12) | ((nextOpe & 0x07FF) << 1);
    return  sprintf(dst, "#0x%08x ; ($+4+0x%08x)", gmAddr + 4 + ofs, ofs);
}

//----------------------------------------------------------------
//  %P  - PC-Relative.
//

#if 0
inline  size_t
writePCRelative(
        const   OpeCode             opeCode,
        char  *  const              dst,
        const   char  *           & src,
        const   GuestMemoryAddress  gmAddr)
{
}
#endif

inline  size_t
writePCRelativeWithVal(
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
    return  sprintf(dst, "[#0x%08x] (=#0x%08x)", pos, val);
}

#if 0

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

#endif

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

inline  size_t
writeRegisterList(
        const   OpeCode     opeCode,
        char  *  const      dst,
        const  char  *    & src)
{
    const  int  regList = (opeCode & 0x00FF);
    int         flgSep  = 0;
    std::stringstream   ss;

    for ( int bit = 0; bit < 8; ) {
        if ( (regList >> bit) & 1 ) {
            const  int  fr = bit;
            while ( (regList >> bit) & 1 ) { ++ bit; }
            const  int  to  = bit - 1;
            if ( flgSep ) {
                ss  <<  ',';
            }
            ss  <<  regNames[fr];
            if ( fr != to ) {
                ss  <<  ((fr == to - 1) ? ',' : '-');
                ss  <<  regNames[to];
            }
            flgSep  = 1;
        } else {
            ++ bit;
        }
    }

    return  sprintf(dst, "%s", ss.str().c_str());
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

    return  sprintf(dst, "#0x%08x ; (0x%08x)", ofs, gmAddr + 4 + ofs);
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

//----------------------------------------------------------------

inline  size_t
writeUnsignedScaleImmediate(
        const   OpeCode     opeCode,
        char  *  const      dst,
        const  char  *    & src)
{
    RegType val = getUnsignedScaleImmediate(opeCode, src);
    int     dig = 4;

    return  sprintf(dst, "0x%0*x", dig, val);
}


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

    OpeCode     op2 = 0;
    if ( (opeCode & 0xF800) == 0xF000 ) {
        op2 = this->m_pManMem->readMemory<OpeCode>(gmAddr + 2);
        sprintf(buf, "%08x:   %04x %04x\t",
                gmAddr, (opeCode & 0xFFFF), (op2 & 0xFFFF)
        );
    } else {
        sprintf(buf, "%08x:   %04x\t", gmAddr, (opeCode & 0xFFFF));
    }
    outStr  <<  buf;

    size_t          len = 0;
    const  char  *  src = oc->mnemonic;
    char  *         dst = buf;
    char            ch;

    while ( (ch = *(src ++)) ) {
        len = 0;
        if ( ch != '%' ) {
            * (dst ++)  = ch;
        } else {
            ch  = *(src ++);
            switch ( ch ) {
            case  'I':
                len = writeUnsignedScaleImmediate(opeCode, dst, src);
                break;
            case  'L':
                len = writeLongOffset(opeCode, op2, dst, src, gmAddr);
                break;
            case  'P':
                len = writePCRelativeWithVal(
                            opeCode, dst, src, *(this->m_pManMem), gmAddr);
                break;
            case  'l':
                len = writeRegisterList(opeCode, dst, src);
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
