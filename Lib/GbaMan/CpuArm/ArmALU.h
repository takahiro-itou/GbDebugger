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
**      An Interface of ArmALU class.
**
**      @file       GbaMan/ArmALU.h
**/

#if !defined( GBDEBUGGER_GBAMAN_CPUARM_INCLUDED_ARM_ALU_H )
#    define   GBDEBUGGER_GBAMAN_CPUARM_INCLUDED_ARM_ALU_H

#if !defined( GBDEBUGGER_GBAMAN_INCLUDED_CPU_UTILS_H )
#    include    "GbDebugger/GbaMan/CpuUtils.h"
#endif

#if !defined( GBDEBUGGER_COMMON_INCLUDED_DEBUGGER_UTILS_H )
#    include    "GbDebugger/Common/DebuggerUtils.h"
#endif

#if !defined( GBDEBUGGER_GBAMAN_CPUARM_INCLUDED_ARITHMETIC_LOGIC_INL )
#    include    "ArithmeticLogic.inl"
#endif

#include    "../Utils/ShiftOperator.h"


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

//  クラスの前方宣言。  //
class   MemoryManager;


//========================================================================
//
//    ArmALU  Instructions.
//

inline  const   RegType
armImmRor(
        const  int      shift,
        const  RegType  vImm)
{
    if ( UNLIKELY(shift) ) {
        return  armRorVal(vImm, shift);
    }
    return ( vImm );
}

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END

#endif
