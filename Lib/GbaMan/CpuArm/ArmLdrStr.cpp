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

#include    <iostream>


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

namespace  {

typedef     GBD_REGPARM     InstExecResult
(* FnLdrStrInst)(
        const  OpeCode  opeCode,
        RegPair         cpuRegs[],
        Memorymanager & manMem,
        RegType       & cpuFlag);

template  <int I, int P, int U, int B, int BIT21, int OP, int SHIFTTYPE,>
GBD_REGPARM     InstExecResult
armLdrStrInstruction(
        const  OpeCode  opeCode,
        RegPair         cpuRegs[],
        Memorymanager & manMem,
        RegType       & cpuFlag)
{
    const  int  rd  = (opeCode >> 12) & 0x0F;
    const  int  rn  = (opeCode >> 16) & 0x0F;
    int         ofs;

    const   bool    flag_cy = (cpuFlag & CPSR::FLAG_C) ? true : false;
    bool            fout_cy = flag_cy;

    if ( I == 0 ) {
        //  オフセットは即値。  //
        ofs = (opeCode & 0x0FFF);
    } else {
        //  オフセットはシフトされたレジスタ。  //
        ofs = getAluOp2Register<SHIFTTYPE, 0>(
                opeCode, cpuRegs, fout_cy, flag_cy);
    }

    //  U フラグ (BIT 23)   //
    if ( U == 0 ) {
        //  オフセットはマイナス。  //
        ofs = - ofs;
    }

    char    buf[512];
    if ( OP == 0 ) {
        //  STR 命令。  //
        sprintf(buf, "Write to address %08x from %d (%08x)",
                cpuRegs[rn] + ofs, rd, cpuRegs[rd]);
    } else {
        //  LDR 命令。  /
        sprintf(buf, "Read from address %08x to %d",
                cpuRegs[rn] + ofs, rd);
    }
    std::cerr   <<  buf <<  std::endl;
}

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
