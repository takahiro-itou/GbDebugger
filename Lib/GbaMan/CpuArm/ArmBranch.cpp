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
**      @file       GbaMan/ArmBranchcpp
**/

#include    "CpuArm.h"


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
//    B 命令
//    cccc_1010_nnnn_nnnn_nnnn_nnnn_nnnn_nnnn (Axx)
//

GBD_REGPARM     InstExecResult
CpuArm::execArmA00_B(
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

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END
