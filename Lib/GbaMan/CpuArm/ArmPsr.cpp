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
#include    "ArmALU.h"


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

namespace  {

inline  const   RegType
generatePsrMask(
        const  OpeCode  opeCode)
{
    RegType maskVal = 0;

    if ( opeCode & 0x00010000) {
        maskVal |= 0x000000FF;
    }
    if ( opeCode & 0x00020000 ) {
        maskVal |= 0x0000FF00;
    }
    if ( opeCode & 0x00040000 ) {
        maskVal |= 0x00FF0000;
    }
    if ( opeCode & 0x00080000 ) {
        maskVal |= 0xFF000000;
    }

    return ( maskVal );
}

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
    mog_cpuRegs[(opeCode >> 12) & 0x0F].dw  = mog_cpuRegs[16].dw;
    return ( InstExecResult::SUCCESS_CONTINUE );
}

GBD_REGPARM     InstExecResult
CpuArm::execArm140_MsrSpsr(
        const  OpeCode  opeCode)
{
    if ( UNLIKELY((opeCode & 0x0FFF0FFF) != 0x014F0FFF) ) {
        return ( InstExecResult::UNDEFINED_OPECODE );
    }
    mog_cpuRegs[(opeCode >> 12) & 0x0F].dw  = mog_cpuRegs[17].dw;
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

    RegType rhs = mog_cpuRegs[(opeCode & 0x0F)].dw;
    RegType val = mog_cpuRegs[16].dw;
    RegType maskVal = generatePsrMask(opeCode);

    //  制御ビットを変更できるのは特権モードの時。  //
    val = (val & ~maskVal) | (rhs & maskVal);
    val |= 0x00000010;

    //  @TODO   CPU モードのスイッチを実装。    //
    mog_cpuRegs[16].dw  = val;
    return ( InstExecResult::SUCCESS_CONTINUE );
}

GBD_REGPARM     InstExecResult
CpuArm::execArm160_MrsSpsrReg(
        const  OpeCode  opeCode)
{
    if ( UNLIKELY((opeCode & 0x0FF0FFF0) != 0x0160F000) ) {
        return ( InstExecResult::UNDEFINED_OPECODE );
    }

    RegType rhs = mog_cpuRegs[(opeCode & 0x0F)].dw;
    RegType val = mog_cpuRegs[17].dw;
    RegType maskVal = generatePsrMask(opeCode);

    //  制御ビットを変更できるのは特権モードの時。  //
    val = (val & ~maskVal) | (rhs & maskVal);
    val |= 0x00000010;

    //  @TODO   CPU モードのスイッチを実装。    //
    mog_cpuRegs[17].dw  = val;
    return ( InstExecResult::SUCCESS_CONTINUE );
}

GBD_REGPARM     InstExecResult
CpuArm::execArm320_MrsCpsrImm(
        const  OpeCode  opeCode)
{
    if ( UNLIKELY((opeCode & 0x0FF0F000) != 0x0320F000) ) {
        return ( InstExecResult::UNDEFINED_OPECODE );
    }

    //  第二オペランドは即値指定。ビット 00..07 で指定される。  //
    RegType rhs = armImmRor((opeCode & 0xF00) >> 7, (opeCode & 0xFF));
    RegType val = mog_cpuRegs[16].dw;
    RegType maskVal = generatePsrMask(opeCode);

    //  制御ビットを変更できるのは特権モードの時。  //
    val = (val & ~maskVal) | (rhs & maskVal);
    val |= 0x00000010;

    //  @TODO   CPU モードのスイッチを実装。    //
    mog_cpuRegs[16].dw  = val;
    return ( InstExecResult::SUCCESS_CONTINUE );
}

GBD_REGPARM     InstExecResult
CpuArm::execArm360_MrsSpsrImm(
        const  OpeCode  opeCode)
{
    if ( UNLIKELY((opeCode & 0x0FF0F000) != 0x0360F000) ) {
        return ( InstExecResult::UNDEFINED_OPECODE );
    }

    //  第二オペランドは即値指定。ビット 00..07 で指定される。  //
    RegType rhs = armImmRor((opeCode & 0xF00) >> 7, (opeCode & 0xFF));
    RegType val = mog_cpuRegs[17].dw;
    RegType maskVal = generatePsrMask(opeCode);

    //  制御ビットを変更できるのは特権モードの時。  //
    val = (val & ~maskVal) | (rhs & maskVal);
    val |= 0x00000010;

    //  @TODO   CPU モードのスイッチを実装。    //
    mog_cpuRegs[17].dw  = val;
    return ( InstExecResult::SUCCESS_CONTINUE );
}

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END
