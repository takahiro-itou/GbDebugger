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
    };
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
        const  uint64_t res,
        const  RegType  lhs,
        const  RegType  rhs,
        const  bool     fout_cy,
        const  RegType  cur)

{
    const  RegType  flag_n  = (res & CPSR::FLAG_N);
    const  RegType  flag_z  = (res ? 0 : CPSR::FLAG_Z);
    const  RegType  flag_c  = (fout_cy)  ? 0x20000000 : 0;

    return ( (cur & 0x1FFFFFFF) | flag_n | flag_z | flag_c );
}

inline  RegType
setCondAdd(
        const  uint64_t res,
        const  RegType  lhs,
        const  RegType  rhs,
        const  bool     fout_cy,
        const  RegType  cur)
{
    const  RegType  flag_n  = (res & CPSR::FLAG_N);
    const  RegType  flag_z  = (res ? 0 : CPSR::FLAG_Z);
    const  RegType  flag_c  = (fout_cy)  ? 0x20000000 : 0;
    const  RegType  work_v  = ~(lhs ^ rhs) & (lhs ^ res) & 0x80000000;
    const  RegType  flag_v  = (work_v >> 3);
    return ( (cur & 0x0FFFFFFF) | flag_n | flag_z | flag_c | flag_v );
}

inline  RegType
setCondSub(
        const  uint64_t res,
        const  RegType  lhs,
        const  RegType  rhs,
        const  bool     fout_cy,
        const  RegType  cur)
{
    const  RegType  flag_n  = (res & CPSR::FLAG_N);
    const  RegType  flag_z  = (res ? 0 : CPSR::FLAG_Z);
    const  RegType  flag_c  = (fout_cy)  ? 0x20000000 : 0;
    const  RegType  work_v  = (lhs ^ rhs) & (lhs ^ res) & 0x80000000;
    const  RegType  flag_v  = (work_v > 0) ? 0x10000000 : 0;
    return ( (cur & 0x0FFFFFFF) | flag_n | flag_z | flag_c | flag_v );
}

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END

#endif
