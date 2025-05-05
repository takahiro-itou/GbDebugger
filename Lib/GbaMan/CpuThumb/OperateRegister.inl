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
**      An Implementation of Operate Register Instructions.
**
**      @file       GbaMan/ThumbOperateRegister.inl
**/

#if !defined( GBDEBUGGER_GBAMAN_INCLUDED_THUMB_OPERATE_REGISTER_INL )
#    define   GBDEBUGGER_GBAMAN_INCLUDED_THUMB_OPERATE_REGISTER_INL

#if !defined( GBDEBUGGER_GBAMAN_INCLUDED_CPU_THUMB_H )
#    include    "CpuThumb.h"
#endif

#if !defined( GBDEBUGGER_GBAMAN_UTILS_INCLUDED_SHIFT_OPERATOR_H )
#    include    "../Utils/ShiftOperator.h"
#endif


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

namespace  {

}   //  End of (Unnamed) namespace.


//========================================================================
//
//    Operate Register Instructions.
//

template  <int OP>
GBD_REGPARM     InstExecResult
CpuThumb::execBitShift(
        const  OpeCode  opeCode)
{
    const  int  rd  = ((opeCode     ) & 0x07);
    const  int  rs  = ((opeCode >> 3) & 0x07);
    const  int  nn  = ((opeCode >> 6) & 0x1F);

    const  RegType  lhs = this->m_cpuRegs[rs].dw;
    RegType         res = lhs;
    RegType         cpuFlag = (this->m_cpuRegs[RegIdx::CPSR].dw);
    bool            flag_cy = (cpuFlag & CPSR::FLAG_C) ? true : false;

    switch ( OP ){
    case  0:
        res = ShiftOpLslImm()(lhs, nn, flag_cy);
        break;
    case  1:
        res = ShiftOpLsrImm()(lhs, nn, flag_cy);
        break;
    case  2:
        res = ShiftOpAsrImm()(lhs, nn, flag_cy);
        break;
    }

    this->m_cpuRegs[RegIdx::CPSR].dw
            = setCondLogical(res, lhs, nn, flag_cy, cpuFlag);
    this->m_cpuRegs[rd].dw  = res;
    return ( InstExecResult::SUCCESS_CONTINUE );
}

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END

#endif
