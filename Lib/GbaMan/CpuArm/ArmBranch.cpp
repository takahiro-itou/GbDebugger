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
**      An Implementation of Branch Instructions.
**
**      @file       GbaMan/ArmBranch.cpp
**/

#include    "CpuArm.h"

#include    "GbDebugger/GbaMan/GbaManager.h"


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

namespace  {

}   //  End of (Unnamed) namespace.


//========================================================================
//
//    Branch  Instructions.
//

//----------------------------------------------------------------
//
//    BX  命令
//    cccc_0001_0010_1111_1111_1111_0001_xxxx (121)
//

GBD_REGPARM     InstExecResult
CpuArm::execArm121_BX(
        const  OpeCode  opeCode)
{
    if ( UNLIKELY((opeCode & 0x0FFFFFF0) != 0x012FFF10) ) {
        return ( InstExecResult::UNDEFINED_OPECODE );
    }

    const  OpeCode  rn  = (opeCode & 0x0F);
    const  RegType  dx  = (this->m_cpuRegs[rn].dw);
    const  RegType  cm  = (dx << 5) & CPSR::FLAG_T;

    if ( cm ) {
        //  THUMB モード。  //
        this->m_nextPC  = dx & ~1;
        this->m_cpuRegs[RegIdx::PC].dw  = (this->m_nextPC + 2);
    } else {
        this->m_nextPC  = dx & ~3;
        this->m_cpuRegs[RegIdx::PC].dw  = (this->m_nextPC + 4);
    }
    this->m_cpuRegs[RegIdx::CPSR].dw  &= ~CPSR::FLAG_T;
    this->m_cpuRegs[RegIdx::CPSR].dw  |=  cm;
    this->m_manGba.changeCpuMode(cm);

    return ( InstExecResult::SUCCESS_CONTINUE );
}

//----------------------------------------------------------------
//
//    B 命令
//    cccc_1010_nnnn_nnnn_nnnn_nnnn_nnnn_nnnn (Axx)
//

GBD_REGPARM     InstExecResult
CpuArm::execArmAxx_B(
        const  OpeCode  opeCode)
{
    //  符号拡張
    //  以下のコードと等価
    //  ofs = (opeCode & 0x00FFFFFF);
    //  if ( ofs & 0x00800000 ) ofs |= 0xFF000000;
    //  ofs <<= 2;
    int32_t ofs = (static_cast<int32_t>(opeCode & 0x00FFFFFF) << 8) >> 6;

    this->m_nextPC  = this->m_cpuRegs[15].dw  += ofs;

    //  プリフェッチを行う。    //
    prefetchAll();

    //  プリフェッチによりカウンタが１命令分進む。  //
    this->m_cpuRegs[15].dw  += 4;

    return ( InstExecResult::SUCCESS_CONTINUE );
}

//----------------------------------------------------------------
//
//    BL  命令
//    cccc_1011_nnnn_nnnn_nnnn_nnnn_nnnn_nnnn (Bxx)
//

GBD_REGPARM     InstExecResult
CpuArm::execArmBxx_BL(
        const  OpeCode  opeCode)
{
    this->m_cpuRegs[14].dw  = this->m_cpuRegs[15].dw - 4;
    return  execArmAxx_B(opeCode);
}

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END
