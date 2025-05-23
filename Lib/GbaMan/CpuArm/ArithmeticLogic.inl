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
**      An Implementation of ALU Instructions.
**
**      @file       GbaMan/CpuArm/ArithmeticLogic.inl
**/

#if !defined( GBDEBUGGER_GBAMAN_CPUARM_INCLUDED_ARITHMETIC_LOGIC_INL )
#    define   GBDEBUGGER_GBAMAN_CPUARM_INCLUDED_ARITHMETIC_LOGIC_INL

#if !defined( GBDEBUGGER_GBAMAN_INCLUDED_CPU_ARM_H )
#    include    "CpuArm.h"
#endif

#if !defined( GBDEBUGGER_GBAMAN_CPUARM_INCLUDED_ARM_ALU_H )
#    include    "ArmALU.h"
#endif

#if !defined( GBDEBUGGER_GBAMAN_UTILS_INCLUDED_SHIFT_OPERATOR_H )
#    include    "../Utils/ShiftOperator.h"
#endif

#include    <iostream>


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

namespace  {

}   //  End of (Unnamed) namespace.


//========================================================================
//
//    第二オペランドの指定に使うファンクタ。
//

struct  ArmALUImmRor
{
    RegType
    operator()(
            const  RegType  vImm,
            const  int      shift,
            bool          & fout_cy)  const
    {
        RegType rhs = vImm;
        if ( UNLIKELY(shift) ) {
            fout_cy = armRorFlg(vImm, shift);
            rhs     = armRorVal(vImm, shift);
        }
        return ( rhs );
    }

    /**   シフトタイプ  **/
    static  CONSTEXPR_VAR   int     SHIFT_TYPE  = 4;

    /**   シフト量は即値指定。  **/
    static  CONSTEXPR_VAR   int     SHIFTW_REG  = 0;
};

//========================================================================
//
//    第二オペランドを決定する。
//

template  <typename SHIFTOP>
inline  const   RegType
getAluOp2Register(
        const  OpeCode  opeCode,
        const  RegPair  cpuRegs[],
        bool          & flagCy)
{
    RegType rhs;

    //  第二オペランドはレジスタ。ビット 00..07 で指定される。  //
    OpeCode iRm = (opeCode & 0x0F);
    RegType vRm = cpuRegs[iRm].dw;

    if ( SHIFTOP::SHIFTW_REG == 0 ) {
        //  シフト量の指定は即値。ビット 07..11 で指定。    //
        const int shift = (opeCode >> 7) & 0x1F;
        //  ビット 05..06 はシフトの種類。  //
        rhs = SHIFTOP()(vRm, shift, flagCy);
    } else {
        //  シフト量指定はレジスタ。ビット 08..11 で指定。  //
        const int shift = cpuRegs[(opeCode >> 8) & 0x0F].dw;
        if ( iRm == 15 ) {
            vRm += 4;       //  オペランド Rm が R15 (PC) の時  //
        }
        //  ビット 05..06 はシフトの種類。  //
        rhs = SHIFTOP()(vRm, shift, flagCy);
    }

    return ( rhs );
}

//========================================================================
//
//    ALU  Instructions.
//

//----------------------------------------------------------------

template  <int BIT25, int CODE, int BIT20, typename SHIFTOP, int BIT4>
GBD_REGPARM     InstExecResult
CpuArm::execArithmeticLogic(
        const  OpeCode  opeCode)
{
    std::cerr   <<  "Not Implemented (ALU)" <<  std::endl;
    return ( InstExecResult::UNDEFINED_OPECODE );
}

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END

#endif
