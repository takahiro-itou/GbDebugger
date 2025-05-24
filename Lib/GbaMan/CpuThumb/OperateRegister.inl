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
**      @file       GbaMan/CpuThumb/OperateRegister.inl
**/

#if !defined( GBDEBUGGER_GBAMAN_CPUTHUMB_INCLUDED_OPERATE_REGISTER_INL )
#    define   GBDEBUGGER_GBAMAN_CPUTHUMB_INCLUDED_OPERATE_REGISTER_INL

#if !defined( GBDEBUGGER_GBAMAN_INCLUDED_CPU_THUMB_H )
#    include    "CpuThumb.h"
#endif

#if !defined( GBDEBUGGER_GBAMAN_UTILS_INCLUDED_SHIFT_OPERATOR_H )
#    include    "../Utils/ShiftOperator.h"
#endif


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

namespace  {

typedef     GBD_REGPARM     void
(* FnALUInst)(
        RegType   & regLhs,
        RegType     regRhs,
        RegType   & cpuFlag);

template  <int OP>
GBD_REGPARM     void
execALUInstruction(
        RegType   & regLhs,
        RegType     regRhs,
        RegType   & cpuFlag)
{
    const  RegType  Cy  = (cpuFlag >> CPSR::FBIT_C) & 1;
    RegType     res = regLhs;
    RegType     flg = cpuFlag;

    switch ( OP ) {
    case  0x00:     //  AND
        res &= regRhs;
        cpuFlag = setCondLogical(res, regLhs, regRhs, flg);
        break;
    case  0x01:     //  EOR
        res ^= regRhs;
        cpuFlag = setCondLogical(res, regLhs, regRhs, flg);
        break;
    case  0x02:     //  LSL
        res <<= (regRhs & 0x00FF);
        cpuFlag = setCondLogical(res, regLhs, regRhs, flg);
        break;
    case  0x03:     //  LSR
        res >>= (regRhs & 0x00FF);
        cpuFlag = setCondLogical(res, regLhs, regRhs, flg);
        break;
    case  0x04:     //  ASR
        res = static_cast<int32_t>(res) >> (regRhs & 0x00FF);
        cpuFlag = setCondLogical(res, regLhs, regRhs, flg);
        break;
    case  0x05:     //  ADC
        res += (regRhs + Cy);
        cpuFlag = setCondAdd(res, regLhs, regRhs, flg);
        break;
    case  0x06:     //  SBC
        res = (res - regRhs + Cy - 1);
        cpuFlag = setCondAdd(res, regLhs, regRhs, flg);
        break;
    case  0x07:     //  ROR
    {
        const  int  sw  = (regRhs & 0x1F);
        if ( sw == 0 ) {
            flg = setCondLogicalCarry(res & 0x80000000, flg);
        } else {
            flg = setCondLogicalCarry(armRorFlg(res, sw), flg);
            res = armRorVal(res, sw);
        }
    }
        cpuFlag = setCondLogical(res, regLhs, regRhs, flg);
        break;
    case  0x08:     //  TST
        cpuFlag = setCondLogical(regLhs & regRhs, regLhs, regRhs, flg);
        break;
    case  0x09:     //  NEG
        res = 0 - regRhs;
        cpuFlag = setCondSub(res, 0, regRhs, flg);
        break;
    case  0x0A:     //  CMP
        cpuFlag = setCondSub(regLhs - regRhs, regLhs, regRhs, flg);
        break;
    case  0x0B:     //  CMN
        cpuFlag = setCondAdd(regLhs + regRhs, regLhs, regRhs, flg);
        break;
    case  0x0C:     //  ORR
        res |= regRhs;
        cpuFlag = setCondLogical(res, regLhs, regRhs, flg);
        break;
    case  0x0D:     //  MUL
        res *= regRhs;
        cpuFlag = setCondLogical(
                        res, regLhs, regRhs, setCondLogicalCarry(0, flg)
        );
        break;
    case  0x0E:     //  BIC
        res &= (~ regRhs);
        cpuFlag = setCondLogical(res, regLhs, regRhs, flg);
        break;
    case  0x0F:     //  MVN
        res =  (~ regRhs);
        cpuFlag = setCondLogical(res, regLhs, regRhs, flg);
        break;
    }

    regLhs  = res;
    return;
}

CONSTEXPR_VAR   FnALUInst
g_thumbALUInstTable[16] = {
    &execALUInstruction<0x00>,
    &execALUInstruction<0x01>,
    &execALUInstruction<0x02>,
    &execALUInstruction<0x03>,
    &execALUInstruction<0x04>,
    &execALUInstruction<0x05>,
    &execALUInstruction<0x06>,
    &execALUInstruction<0x07>,
    &execALUInstruction<0x08>,
    &execALUInstruction<0x09>,
    &execALUInstruction<0x0A>,
    &execALUInstruction<0x0B>,
    &execALUInstruction<0x0C>,
    &execALUInstruction<0x0D>,
    &execALUInstruction<0x0E>,
    &execALUInstruction<0x0F>,
};

}   //  End of (Unnamed) namespace.


//========================================================================
//
//    Operate Register Instructions.
//

//----------------------------------------------------------------

inline
GBD_REGPARM     InstExecResult
CpuThumb::execArithmeticLogic(
        const  OpeCode  opeCode)
{
    const  int  rd  = ((opeCode     ) & 0x07);
    const  int  rs  = ((opeCode >> 3) & 0x07);
    const  int  idx = ((opeCode >> 6) & 0x0F);

    FnALUInst   pfInst  = g_thumbALUInstTable[idx];

    (* pfInst)(
            mog_cpuRegs[rd].dw,
            mog_cpuRegs[rs].dw,
            mog_cpuRegs[16].dw);
    return ( InstExecResult::SUCCESS_CONTINUE );
}

//----------------------------------------------------------------

template  <int OP>
GBD_REGPARM     InstExecResult
CpuThumb::execBitShift(
        const  OpeCode  opeCode)
{
    const  int  rd  = ((opeCode     ) & 0x07);
    const  int  rs  = ((opeCode >> 3) & 0x07);
    const  int  nn  = ((opeCode >> 6) & 0x1F);

    const  RegType  lhs = mog_cpuRegs[rs].dw;
    RegType         res = lhs;
    RegType         cpuFlag = (mog_cpuRegs[RegIdx::CPSR].dw);
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

    cpuFlag = setCondLogicalCarry(flag_cy, cpuFlag);
    mog_cpuRegs[RegIdx::CPSR].dw
            = setCondLogical(res, lhs, nn, cpuFlag);
    mog_cpuRegs[rd].dw  = res;
    return ( InstExecResult::SUCCESS_CONTINUE );
}

//----------------------------------------------------------------

template  <int OP>
GBD_REGPARM     InstExecResult
CpuThumb::execOperateAddSub(
        const  OpeCode  opeCode)
{
    const  int  rd  = ((opeCode     ) & 0x07);
    const  int  rs  = ((opeCode >> 3) & 0x07);
    const  int  nn  = ((opeCode >> 6) & 0x07);

    const  RegType  lhs = (mog_cpuRegs[rs].dw);
    const  RegType  cur = (mog_cpuRegs[16].dw);
    RegType         rhs, res, flg;

    switch ( OP ) {
    case  0:    //  ADD Rd, Rs, Rn
        rhs = mog_cpuRegs[nn].dw;
        flg = setCondAdd((res = lhs + rhs), lhs, rhs, cur);
        break;
    case  1:    //  SUB Rd, Rs, Rn
        rhs = mog_cpuRegs[nn].dw;
        flg = setCondSub((res = lhs - rhs), lhs, rhs, cur);
        break;
    case  2:    //  ADD Rd, Rs, #nn
        rhs = nn;
        flg = setCondAdd((res = lhs + rhs), lhs, rhs, cur);
        break;
    case  3:    //  SUB RD, Rs, #nn
        rhs = nn;
        flg = setCondSub((res = lhs - rhs), lhs, rhs, cur);
        break;
    }

    mog_cpuRegs[rd].dw  = res;
    mog_cpuRegs[16].dw  = flg;

    return ( InstExecResult::SUCCESS_CONTINUE );
}

//----------------------------------------------------------------

template  <int OP>
GBD_REGPARM     InstExecResult
CpuThumb::execOperateHighRegister(
        const  OpeCode  opeCode)
{
    const  int  rd  = ((opeCode >> 4) & 0x08) | (opeCode & 0x07);
    const  int  rs  = ((opeCode >> 3) & 0x0F);

    switch ( OP ) {
    case  0:    //  ADD
        mog_cpuRegs[rd].dw  += mog_cpuRegs[rs].dw;
        break;
    case  1:    //  CMP
    {
        RegType flg = mog_cpuRegs[RegIdx::CPSR].dw;
        const  RegType  lhs = mog_cpuRegs[rd].dw;
        const  RegType  rhs = mog_cpuRegs[rs].dw;
        const  RegType  res = lhs - rhs;
        mog_cpuRegs[RegIdx::CPSR].dw = setCondSub(res, lhs, rhs, flg);
    }
        break;
    case  2:    //  MOV
        mog_cpuRegs[rd].dw  = mog_cpuRegs[rs].dw;
        break;
    case  3:    //  BX
        modifyProgramCounter(mog_cpuRegs[rs].dw);
        break;
    }

    return ( InstExecResult::SUCCESS_CONTINUE );
}

//----------------------------------------------------------------

template  <int OP, int RD>
GBD_REGPARM     InstExecResult
CpuThumb::execOperateImm(
        const  OpeCode  opeCode)
{
    const  RegType  lhs = (mog_cpuRegs[RD].dw);
    const  RegType  rhs = (opeCode & 0x00FF);
    RegType     res;

    const  RegType  cur = (mog_cpuRegs[16].dw);
    RegType         flg;

    switch ( OP ) {
    case  0x00:     //  MOV Rd, #nn
        res = rhs;
        flg = setCondLogical(res, lhs, rhs, cur);
        mog_cpuRegs[RD].dw  = res;
        break;
    case  0x01:     //  CMP Rd, #nn
        res = lhs - rhs;
        flg = setCondSub(res, lhs, rhs, cur);
        break;
    case  0x02:     //  ADD Rd, #nn
        res = lhs + rhs;
        flg = setCondAdd(res, lhs, rhs, cur);
        mog_cpuRegs[RD].dw  = res;
        break;
    case  0x03:     //  SUB Rd, #nn
        res = lhs - rhs;
        flg = setCondSub(res, lhs, rhs, cur);
        mog_cpuRegs[RD].dw  = res;
        break;
    }

    mog_cpuRegs[RegIdx::CPSR].dw    = flg;
    return ( InstExecResult::SUCCESS_CONTINUE );
}

//----------------------------------------------------------------

inline
GBD_REGPARM     InstExecResult
CpuThumb::execOperateSP(
        const  OpeCode  opeCode)
{
    const   GuestMemoryAddress  nn  = (opeCode << 2) & 0x01FF;
    mog_cpuRegs[RegIdx::SP].dw  += ((opeCode & 0x0080) ? -nn : nn);

    return ( InstExecResult::SUCCESS_CONTINUE );
}

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END

#endif
