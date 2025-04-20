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
**      An Implementation of ArmALU class.
**
**      @file       GbaMan/ArmALU.cpp
**/

#include    "ArmALU.h"


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

namespace  {

}   //  End of (Unnamed) namespace.


template  <int  BIT25, int CODE, int BIT20, int SHIFTTYPE, int BIT4>
GBD_REGPARM     InstExecResult
CpuArm::armALUInstruction(
        const  OpeCode  opeCode,
        RegPair         cpuRegs,
        uint32_t      & cpuFlag)
{
    //  結果を格納するレジスタはビット 12..15 で指定。  //
    const  int      dst = (opeCode >> 12) & 0x0F;

    //  第一オペランドレジスタはビット 16..19 で指定。  //
    const  uint32_t lhs = this->m_cpuRegs[(opeCode >> 16) & 0x0F].dw;

    uint32_t        rhs;

    const   bool    flag_cy = (cpuFlag & 0x20000000) ? true : false;
    bool            fout_cy = flag_cy;
}

//========================================================================
//
//    ArmALU  Instructions.
//

GBD_REGPARM     InstExecResult
armALUInstruction(
        const  OpeCode  opeCode,
        RegPair         cpuRegs,
        uint32_t      & cpuFlag)
{
    return ( InstExecResult::UNDEFINED_OPECODE );
}

//========================================================================
//
//    ArmALU  class.
//

//========================================================================
//
//    Constructor(s) and Destructor.
//

//========================================================================
//
//    Public Member Functions (Implement Pure Virtual).
//

//========================================================================
//
//    Public Member Functions (Overrides).
//

//========================================================================
//
//    Public Member Functions (Pure Virtual Functions).
//

//========================================================================
//
//    Public Member Functions (Virtual Functions).
//

//========================================================================
//
//    Public Member Functions.
//

//========================================================================
//
//    Accessors.
//

//========================================================================
//
//    Protected Member Functions.
//

//========================================================================
//
//    For Internal Use Only.
//

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END
