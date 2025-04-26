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
**      An Implementation of ArmLdrStr Instructions.
**
**      @file       GbaMan/ArmLdrStr.cpp
**/

#include    "CpuArm.h"


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

namespace  {

typedef     GBD_REGPARM     InstExecResult
(* FnLdrStrInst)(
        const  OpeCode  opeCode,
        RegPair         cpuRegs[],
        Memorymanager & manMem,
        RegType       & cpuFlag);

}   //  End of (Unnamed) namespace.


//========================================================================
//
//    ArmLdrStr  Instructions.
//

GBD_REGPARM     InstExecResult
CpuArm::armLdrStrInstruction(
        const  OpeCode  opeCode)
{
    return ( InstExecResult::UNDEFINED_OPECODE );
}

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END
