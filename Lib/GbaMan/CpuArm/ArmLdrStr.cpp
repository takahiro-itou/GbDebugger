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

template  <int I, int P, int U, int B, int BIT21, int SHIFTTYPE, int OP>
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
        OpeCode iRm = (opeCode & 0x0F);
        RegType vRm = cpuRegs[iRm].dw;

        const int shift = (opeCode >> 7) & 0x1F;

        //  ビット 05..06 はシフトの種類。  //
        switch ( SHIFTTYPE ) {
        case  0:    //  LSL
            ofs = ArmALURmLslImm()(shift, vRm, fout_cy, flag_cy);
            break;
        case  1:    //  LSR
            ofs = ArmALURmLsrImm()(shift, vRm, fout_cy, flag_cy);
            break;
        case  2:    //  ASR
            ofs = ArmALURmAsrImm()(shift, vRm, fout_cy, flag_cy);
            break;
        case  3:    //  ROR
            ofs = ArmALURmRorImm()(shift, vRm, fout_cy, flag_cy);
            break;
        }
    }

    //  U フラグ (BIT 23)   //
    if ( U == 0 ) {
        //  オフセットはマイナス。  //
        ofs = - ofs;
    }

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
