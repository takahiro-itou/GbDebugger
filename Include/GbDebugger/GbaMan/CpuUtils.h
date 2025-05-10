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
**      Utility Macros/Functions.
**
**      @file       Common/CpuUtils.h
**/

#if !defined( GBDEBUGGER_GBAMAN_INCLUDED_CPU_UTILS_H )
#    define   GBDEBUGGER_GBAMAN_INCLUDED_CPU_UTILS_H

#if !defined( GBDEBUGGER_COMMON_INCLUDED_DEBUGGER_TYPES_H )
#    include    "GbDebugger/Common/DebuggerTypes.h"
#endif


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {


//========================================================================
//
//    Type Definitions.
//

/**
**    命令の実行結果。
**/

enum class  InstExecResult
{
    /**
    **    正常に実行された。
    **
    **    インタープリターは引き続き次の命令を実行する。
    **/
    SUCCESS_CONTINUE    = 0,

    /**
    **    未定義の命令が実行され、プロセッサが停止した。
    **/
    UNDEFINED_OPECODE   = 1,

    /**
    **    正常に実行されたが、ブレークポイントにヒットした。
    **/
    SUCCESS_BREAKPOINT  = 2,
};


//========================================================================
//
//    Register Banks.
//

struct  RegIdx  {
    enum  {
        //  通常のレジスタ群。  //
        R0      = 0,
        R1      = 1,
        R2      = 2,
        R3      = 3,
        R4      = 4,
        R5      = 5,
        R6      = 6,
        R7      = 7,
        R8      = 8,
        R9      = 9,
        R10     = 10,
        R11     = 11,
        R12     = 12,
        R13     = 13,
        R14     = 14,
        PC      = 15,
        CPSR    = 16,

        //  FIQ バンク。    //
        R8_fiq  = 17,
        R8_usr  = 18,
        R9_fiq  = 19,
        R9_usr  = 20,
        R10fiq  = 21,
        R10usr  = 22,
        R11fiq  = 23,
        R11usr  = 24,
        R12fiq  = 25,
        R12usr  = 26,

        //  R13 R14 バンク  //
        R13fiq  = 27,
        R14fiq  = 28,
        R13svc  = 29,
        R14svc  = 30,
        R13abt  = 31,
        R14abt  = 32,
        R13irq  = 33,
        R14irq  = 34,
        R13und  = 35,
        R14und  = 36,
        R13usr  = 37,
        R14usr  = 38,

        //  SPSR バンク。   //
        SPSRfiq = 39,
        SPSRsvc = 40,
        SPSRabt = 41,
        SPSRirq = 42,
        SPSRund = 43,
        SPSRusr = 44,

        NEXT_PC = 45,
        NUM_REGISTERS   = 46,
    };
};

typedef     RegPair     RegBank[RegIdx::NUM_REGISTERS];


//========================================================================
//
//    CPSR (Flag) Register.
//

struct  CPSR  {
    enum  {
        FLAG_N      = 0x80000000,
        FLAG_Z      = 0x40000000,
        FLAG_C      = 0x20000000,
        FLAG_V      = 0x10000000,
        FLAG_Q      = 0x08000000,   //  bit  27 : GBA では未使用
        RESERVED    = 0x07FFFF00,   //  bit26-8 : 予約
        FLAG_I      = 0x00000080,   //  bit   7 : 1=IRQ 無効
        FLAG_F      = 0x00000040,   //  bit   6 : 1=FIQ 無効
        FLAG_T      = 0x00000020,   //  bit   5 : THUMB (0=ARM, 1=THUMB)
        MODE_MASK   = 0x0000001F,   //  bit 0-4 : モード

        //  モード  //
        MODE_USER   = 0x10,
        MODE_FIQ    = 0x11,
        MODE_IRQ    = 0x12,
        MODE_SWI    = 0x13, //  Supervisor
        MODE_ABORT  = 0x17,
        MODE_UNDEF  = 0x1B,
        MODE_SYSTEM = 0x1F,

        FBIT_N      = 31,
        FBIT_Z      = 30,
        FBIT_C      = 29,
        FBIT_V      = 28,
    };
};

//========================================================================
/**
**    条件判定用のテーブル。
**
**    フラグは NZCV の 4bit あるから 0...15 の整数値とみなして
**  各条件についてどの数値だったら真とみなすかをテーブルにする。
**
**  例えば EQ (Z==1) であれば
**  0100, 0101, 0110, 0111, 1100, 1101, 1110, 1111
**  のいずれかであれば真になるから、
**  配列の 4, 5, 6, 7, 12, 13, 14, 15 に真を、それ以外に偽を書き込んだ
**  テーブルを用意する。
**/

CONSTEXPR_VAR   int
g_condTable[16][16] = {
    // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F
    {  0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1 },    //  EQ (F0F0)
    {  1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },    //  NE (0F0F)
    {  0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1 },    //  CS (CCCC)
    {  1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0 },    //  CC (3333)
    {  0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1 },    //  MI (FF00)
    {  1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 },    //  PL (00FF)
    {  0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1 },    //  VS (AAAA)
    {  1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0 },    //  VC (5555)
    {  0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0 },    //  HI (0C0C)
    {  1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1 },    //  LS (F3F3)
    {  1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1 },    //  GE (AA55)
    {  0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0 },    //  LT (55AA)
    {  1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0 },    //  GT (0A05)
    {  0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1 },    //  LE (F5FA)
    {  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },    //  AL (FFFF)
    {  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }     //  NV (0000)
};

//========================================================================
//
//    Rotate Instruction.
//

CONSTEXPR_FUNC  inline  bool
armRorFlg(
        const  RegType  v,
        const  int      shift)
{
    return ( (v >> (shift - 1)) & 1 ? true : false );
}

CONSTEXPR_FUNC  inline  RegType
armRorVal(
        const  RegType  v,
        const  int      shift)
{
    return  ((v << (32 - shift)) | (v >> shift));
}

//----------------------------------------------------------------

inline  RegType
setCondLogical(
        const  RegType  res,
        const  RegType  lhs,
        const  RegType  rhs,
        const  bool     fout_cy,
        const  RegType  cur)

{
    const  RegType  flag_n  = (res & CPSR::FLAG_N);
    const  RegType  flag_z  = (res ? 0 : CPSR::FLAG_Z);
    const  RegType  flag_c  = (fout_cy ? CPSR::FLAG_C : 0);

    return ( (cur & 0x1FFFFFFF) | flag_n | flag_z | flag_c );
}

inline  RegType
setCondAdd(
        const  RegType  res,
        const  RegType  lhs,
        const  RegType  rhs,
        const  bool,
        const  RegType  cur)
{
    const  RegType  flag_n  = (res & CPSR::FLAG_N);
    const  RegType  flag_z  = (res ? 0 : CPSR::FLAG_Z);

    const  RegType  work_c  = (lhs & rhs) | ((lhs | rhs) & ~res);
    const  RegType  flag_c  = (work_c >> 2) & CPSR::FLAG_C;

    const  RegType  work_v  = ~(lhs ^ rhs) & (lhs ^ res);
    const  RegType  flag_v  = (work_v >> 3) & CPSR::FLAG_V;

    return ( (cur & 0x0FFFFFFF) | flag_n | flag_z | flag_c | flag_v );
}

inline  RegType
setCondSub(
        const  RegType  res,
        const  RegType  lhs,
        const  RegType  rhs,
        const  bool,
        const  RegType  cur)
{
    const  RegType  flag_n  = (res & CPSR::FLAG_N);
    const  RegType  flag_z  = (res ? 0 : CPSR::FLAG_Z);

    const  RegType  work_c  = (lhs & ~rhs) | ((lhs | ~rhs) & ~res);
    const  RegType  flag_c  = (work_c >> 2) & CPSR::FLAG_C;

    const  RegType  work_v  = (lhs ^ rhs) & (lhs ^ res);
    const  RegType  flag_v  = (work_v >> 3) & CPSR::FLAG_V;

    return ( (cur & 0x0FFFFFFF) | flag_n | flag_z | flag_c | flag_v );
}

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END

#endif
