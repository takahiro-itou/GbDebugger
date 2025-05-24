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

#if !defined( GBDEBUGGER_SYS_INCLUDED_CASSERT )
#    include    <cassert>
#    define   GBDEBUGGER_SYS_INCLUDED_CASSERT
#endif

#if !defined( GBDEBUGGER_SYS_INCLUDED_IOSTREAM )
#    include    <iostream>
#    define   GBDEBUGGER_SYS_INCLUDED_IOSTREAM
#endif


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

template  <int BIT25, AluOp CODE, int BIT20, typename SHIFTOP, int BIT4>
GBD_REGPARM     InstExecResult
CpuArm::execArithmeticLogic(
        const  OpeCode  opeCode)
{
    RegType   & cpuFlag = mog_cpuRegs[RegIdx::CPSR].dw;

    //  結果を格納するレジスタはビット 12..15 で指定。  //
    const  int      dst = (opeCode >> 12) & 0x0F;

#if ( GBDEBUGGER_ENABLE_TRACELOG )
    char    buf[512];
#if defined( __GNUC__ )
    std::cerr   <<  __PRETTY_FUNCTION__ <<  std::endl;
#endif
    sprintf(buf,
            "Op2(I/R) = %d, CODE = %x, S = %d, SHIFT = %d/%d, BIT4(R) = %d\n",
            BIT25, static_cast<int>(CODE), BIT20,
            SHIFTOP::SHIFT_TYPE, SHIFTOP::SHIFTW_REG, BIT4);
    std::cerr   <<  buf;
    sprintf(buf,
            "OpeCode = %08x, dst = %d\n",
            opeCode, dst);
    std::cerr   <<  buf;
    assert( SHIFTOP::SHIFTW_REG == BIT4 );
#endif

    //  第一オペランドレジスタはビット 16..19 で指定。  //
    const  RegType  lhs = mog_cpuRegs[(opeCode >> 16) & 0x0F].dw;
    const  RegType  Cy  = (cpuFlag >> CPSR::FBIT_C) & 0x01;
    RegType     rhs;
    RegType     res     = mog_cpuRegs[dst].dw;
    bool        flagCy  = (Cy ? true : false);

    static_assert(
            (BIT25 == 0) || (std::is_same<SHIFTOP, ArmALUImmRor>::value),
            "SHIFTOP must be ArmALUImmRor if BIT25 == 1"
    );

    if ( BIT25 == 0 ) {
        //  第二オペランドはレジスタ。ビット 00..07 で指定される。  //
#if 0
        if ( BIT4 == 0 ) {
            rhs = getAluOp2Register<SHIFTOP, BIT4>(
                    opeCode, cpuRegs, flagCy);
        } else{
            rhs = getAluOp2Register<SHIFTOP, BIT4>(
                    opeCode, cpuRegs, flagCy);
        }
#endif
        rhs = getAluOp2Register<SHIFTOP>(
                    opeCode, mog_cpuRegs, flagCy);
    } else {
        //  第二オペランドは即値指定。ビット 00..07 で指定される。  //
        const  RegType  imm = (opeCode & 0xFF);
        const  int      ror = (opeCode & 0xF00) >> 7;
        rhs = SHIFTOP()(imm, ror, flagCy);
    }

    const  RegType  cur = setCondLogicalCarry(flagCy, cpuFlag);
    RegType         flg;

    switch ( CODE ) {
    case  AluOp::AND:       //  AND         Rd = Rn AND Op2
        flg = setCondLogical((res = lhs & rhs), lhs, rhs, cur);
        mog_cpuRegs[dst].dw = res;
        break;
    case  AluOp::EOR:       //  EOR (XOR)   Rd = Rn XOR Op2
        flg = setCondLogical((res = lhs ^ rhs), lhs, rhs, cur);
        mog_cpuRegs[dst].dw = res;
        break;
    case  AluOp::SUB:       //  SUB         Rd = Rn - OP2
        flg = setCondSub((res = lhs - rhs), lhs, rhs, cur);
        mog_cpuRegs[dst].dw = res;
        break;
    case  AluOp::RSB:       //  RSB         Rd = Op2 - Rn
        flg = setCondSub((res = rhs - lhs), rhs, lhs, cur);
        mog_cpuRegs[dst].dw = res;
        break;
    case  AluOp::ADD:       //  ADD         Rd = Rn + Op2
        flg = setCondAdd((res = lhs + rhs), lhs, rhs, cur);
        mog_cpuRegs[dst].dw = res;
        break;
    case  AluOp::ADC:       //  ADC         Rd = Rn + Op2 + Cy
        flg = setCondAdd((res = lhs + rhs + Cy), lhs, rhs, cur);
        mog_cpuRegs[dst].dw = res;
        break;
    case  AluOp::SBC:       //  SBC         Rd = Rn - Op2 + Cy - 1
        flg = setCondAdd((res = lhs - rhs + Cy - 1), lhs, rhs, cur);
        mog_cpuRegs[dst].dw = res;
        break;
    case  AluOp::RSC:       //  RSC         Rd = Op2 - Rn + Cy - 1
        flg = setCondAdd((res = rhs - lhs + Cy - 1), rhs, lhs, cur);
        mog_cpuRegs[dst].dw = res;
        break;
    case  AluOp::TST:       //  TST         (void)(Rn AND Op2)
        flg = setCondLogical((lhs & rhs), lhs, rhs, cur);
        break;
    case  AluOp::TEQ:       //  TEQ         (void)(Rn XOR Op2)
        flg = setCondLogical((lhs ^ rhs), lhs, rhs, cur);
        break;
    case  AluOp::CMP:       //  CMP         (void)(Rn - Op2)
        flg = setCondSub((lhs - rhs), lhs, rhs, cur);
        break;
    case  AluOp::CMN:       //  CMN         (void)(Rn + Op2)
        flg = setCondAdd((lhs + rhs), lhs, rhs, cur);
        break;
    case  AluOp::ORR:       //  ORR (OR)    Rd = Rn OR Op2
        flg = setCondLogical((res = lhs | rhs), lhs, rhs, cur);
        mog_cpuRegs[dst].dw = res;
        break;
    case  AluOp::MOV:       //  MOV         Rd = Op2
        flg = setCondLogical((res = rhs), lhs, rhs, cur);
        mog_cpuRegs[dst].dw = res;
        break;
    case  AluOp::BIC:       //  BIC         Rd = Rnn AND NOT Op2
        flg = setCondLogical((res = lhs & ~rhs), lhs, rhs, cur);
        mog_cpuRegs[dst].dw = res;
        break;
    case  AluOp::MVN:       //  MVN         Rd = Not Op2
        flg = setCondLogical((res = ~rhs), lhs, rhs, cur);
        mog_cpuRegs[dst].dw = res;
        break;
    }

    if ( LIKELY(dst) == 0x0F ) {
        if ( BIT20 == 1 ) {
            //  フラグレジスタを更新する。  //
            cpuFlag = flg;
        }
    } else {
        //  モードの変更処理。  //
    }

    return ( InstExecResult::SUCCESS_CONTINUE );
}

//----------------------------------------------------------------

template  <MulOp OP, int S>
GBD_REGPARM     InstExecResult
CpuArm::execMultiplyHalf(
        const  OpeCode  opeCode)
{
    std::cerr   <<  "Unsupported (Multiply Half)"   <<  std::endl;
    return ( InstExecResult::UNDEFINED_OPECODE );
}

template  <MulOp OP, int S>
GBD_REGPARM     InstExecResult
CpuArm::execMultiplyWord(
        const  OpeCode  opeCode)
{
    std::cerr   <<  "Not Implemented (Multiply Word)"   <<  std::endl;
    return ( InstExecResult::UNDEFINED_OPECODE );
}

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END

#endif
