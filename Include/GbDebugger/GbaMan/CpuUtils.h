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
    const  RegType  flag_n  = (res & 0x80000000);
    const  RegType  flag_z  = (res == 0) ? 0x40000000 : 0;
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
    const  RegType  flag_n  = (res & 0x80000000);
    const  RegType  flag_z  = (res == 0) ? 0x40000000 : 0;
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
    const  RegType  flag_n  = (res & 0x80000000);
    const  RegType  flag_z  = (res == 0) ? 0x40000000 : 0;
    const  RegType  flag_c  = (fout_cy)  ? 0x20000000 : 0;
    const  RegType  work_v  = (lhs ^ rhs) & (lhs ^ res) & 0x80000000;
    const  RegType  flag_v  = (work_v > 0) ? 0x10000000 : 0;
    return ( (cur & 0x0FFFFFFF) | flag_n | flag_z | flag_c | flag_v );
}

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END

#endif
