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
**      An Implementation of PSR Transfer Instructions.
**
**      @file       GbaMan/ArmPsr.cpp
**/

#include    "CpuArm.h"


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

namespace  {

}   //  End of (Unnamed) namespace.


//========================================================================
//
//    PSR Transfer  Instructions.
//

//========================================================================
//
//    MRS  Instructions.
//

GBD_REGPARM     InstExecResult
CpuArm::execArm100_MsrCpsr(
        const  OpeCode  opeCode)
{
    if ( UNLIKELY((opeCode & 0x0FFF0FFF) != 0x010F0FFF) ) {
        return ( InstExecResult::UNDEFINED_OPECODE );
    }
    this->m_cpuRegs[(opeCode >> 12) & 0x0F].dw  = this->m_cpuRegs[16].dw;
    return ( InstExecResult::SUCCESS_CONTINUE );
}

GBD_REGPARM     InstExecResult
CpuArm::execArm140_MsrSpsr(
        const  OpeCode  opeCode)
{
    if ( UNLIKELY((opeCode & 0x0FFF0FFF) != 0x014F0FFF) ) {
        return ( InstExecResult::UNDEFINED_OPECODE );
    }
    this->m_cpuRegs[(opeCode >> 12) & 0x0F].dw  = this->m_cpuRegs[17].dw;
    return ( InstExecResult::SUCCESS_CONTINUE );
}

//========================================================================
//
//    MRS Instructions.
//

GBD_REGPARM     InstExecResult
CpuArm::execArm120_MrsCpsrReg(
        const  OpeCode  opeCode)
{
    if ( UNLIKELY((opeCode & 0x0FF0FFF0) != 0x0120F000) ) {
        return ( InstExecResult::UNDEFINED_OPECODE );
    }
}

GBD_REGPARM     InstExecResult
CpuArm::execArm160_MrsSpsrReg(
        const  OpeCode  opeCode)
{
    if ( UNLIKELY((opeCode & 0x0FF0FFF0) != 0x0160F000) ) {
        return ( InstExecResult::UNDEFINED_OPECODE );
    }

}

GBD_REGPARM     InstExecResult
CpuArm::execArm320_MrsCpsrImm(
        const  OpeCode  opeCode)
{
    if ( UNLIKELY((opeCode & 0x0FF0F000) != 0x0320F000) ) {
        return ( InstExecResult::UNDEFINED_OPECODE );
    }
}

GBD_REGPARM     InstExecResult
CpuArm::execArm360_MrsSpsrImm(
        const  OpeCode  opeCode)
{
    if ( UNLIKELY((opeCode & 0x0FF0F000) != 0x0360F000) ) {
        return ( InstExecResult::UNDEFINED_OPECODE );
    }
}

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END
