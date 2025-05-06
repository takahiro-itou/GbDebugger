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
#include    "CpuArm.h"

#include    <cassert>
#include    <iostream>
#include    <stdio.h>


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

namespace  {

typedef     GBD_REGPARM     InstExecResult
(* FnALUInst)(
        const  OpeCode  opeCode,
        RegPair         cpuRegs[],
        RegType       & cpuFlag);

template  <int BIT25, int CODE, int BIT20, typename SHIFTOP, int BIT4>
GBD_REGPARM     InstExecResult
armALUInstruction(
        const  OpeCode  opeCode,
        RegPair         cpuRegs[],
        RegType       & cpuFlag)
{
    //  結果を格納するレジスタはビット 12..15 で指定。  //
    const  int      dst = (opeCode >> 12) & 0x0F;

    char    buf[512];
#if defined( __GNUC__ )
    std::cerr   <<  __PRETTY_FUNCTION__ <<  std::endl;
#endif
    sprintf(buf,
            "Op2(I/R) = %d, CODE = %x, S = %d, SHIFT = %d/%d, BIT4(R) = %d\n",
            BIT25, CODE, BIT20,
            SHIFTOP::SHIFT_TYPE, SHIFTOP::SHIFTW_REG, BIT4);
    std::cerr   <<  buf;
    sprintf(buf,
            "OpeCode = %08x, dst = %d\n",
            opeCode, dst);
    std::cerr   <<  buf;
    assert( SHIFTOP::SHIFTW_REG == BIT4 );

    //  第一オペランドレジスタはビット 16..19 で指定。  //
    const  RegType  lhs = cpuRegs[(opeCode >> 16) & 0x0F].dw;
    const  RegType  Cy  = (cpuFlag >> CPSR::FBIT_C) & 0x01;
    RegType     rhs;
    uint64_t    res;
    bool        flagCy = (Cy ? true : false);

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
        rhs = getAluOp2Register<SHIFTOP, SHIFTOP::SHIFTW_REG>(
                opeCode, cpuRegs, flagCy);
    } else {
        //  第二オペランドは即値指定。ビット 00..07 で指定される。  //
        const  RegType  imm = (opeCode & 0xFF);
        const  int      ror = (opeCode & 0xF00) >> 7;
        rhs = SHIFTOP()(imm, ror, flagCy);
    }

    const  RegType  cur = cpuFlag;
    RegType         flg;

    switch ( CODE ) {
    case  0x00:     //  AND         Rd = Rn AND Op2
        res = lhs & rhs;
        flg = setCondLogical(res, lhs, rhs, flagCy, cur);
        cpuRegs[dst].dw = res;
        break;
    case  0x01:     //  EOR (XOR)   Rd = Rn XOR Op2
        res = lhs ^ rhs;
        flg = setCondLogical(res, lhs, rhs, flagCy, cur);
        cpuRegs[dst].dw = res;
        break;
    case  0x02:     //  SUB         Rd = Rn - OP2
        res = static_cast<uint64_t>(lhs) - static_cast<uint64_t>(rhs);
        flg = setCondSub(res, lhs, rhs, flagCy, cur);
        cpuRegs[dst].dw = res;
        break;
    case  0x03:     //  RSB         Rd = Op2 - Rn
        res = static_cast<uint64_t>(rhs) - static_cast<uint64_t>(lhs);
        flg = setCondSub(res, rhs, lhs, flagCy, cur);
        cpuRegs[dst].dw = res;
        break;
    case  0x04:     //  ADD         Rd = Rn + Op2
        res = static_cast<uint64_t>(lhs) + static_cast<uint64_t>(rhs);
        flg = setCondAdd(res, lhs, rhs, flagCy, cur);
        cpuRegs[dst].dw = res;
        break;
    case  0x05:     //  ADC         Rd = Rn + Op2 + Cy
        res = static_cast<uint64_t>(lhs) + static_cast<uint64_t>(rhs)
                    + static_cast<uint64_t>(Cy);
        flg = setCondAdd(res, lhs, rhs, flagCy, cur);
        cpuRegs[dst].dw = res;
        break;
    case  0x06:     //  SBC         Rd = Rn - Op2 + Cy - 1
        res = static_cast<uint64_t>(lhs) - static_cast<uint64_t>(rhs)
                    + static_cast<uint64_t>(Cy - 1);
        flg = setCondAdd(res, lhs, rhs, flagCy, cur);
        cpuRegs[dst].dw = res;
        break;
    case  0x07:     //  RSC         Rd = Op2 - Rn + Cy - 1
        res = static_cast<uint64_t>(rhs) - static_cast<uint64_t>(lhs)
                    + static_cast<uint64_t>(Cy - 1);
        flg = setCondAdd(res, rhs, lhs, flagCy, cur);
        cpuRegs[dst].dw = res;
        break;
    case  0x08:     //  TST         (void)(Rn AND Op2)
        res = lhs & rhs;
        flg = setCondLogical(res, lhs, rhs, flagCy, cur);
        break;
    case  0x09:     //  TEQ         (void)(Rn XOR Op2)
        res = lhs ^ rhs;
        flg = setCondLogical(res, lhs, rhs, flagCy, cur);
        break;
    case  0x0A:     //  CMP         (void)(Rn - Op2)
        res = static_cast<uint64_t>(lhs) - static_cast<uint64_t>(rhs);
        flg = setCondSub(res, lhs, rhs, flagCy, cur);
        break;
    case  0x0B:     //  CMN         (void)(Rn + Op2)
        res = static_cast<uint64_t>(lhs) + static_cast<uint64_t>(rhs);
        flg = setCondAdd(res, lhs, rhs, flagCy, cur);
        break;
    case  0x0C:     //  ORR (OR)    Rd = Rn OR Op2
        res = lhs | rhs;
        flg = setCondLogical(res, lhs, rhs, flagCy, cur);
        cpuRegs[dst].dw = res;
        break;
    case  0x0D:     //  MOV         Rd = Op2
        res = rhs;
        flg = setCondLogical(res, lhs, rhs, flagCy, cur);
        cpuRegs[dst].dw = res;
        break;
    case  0x0E:     //  BIC         Rd = Rnn AND NOT Op2
        res = lhs & ~rhs;
        flg = setCondLogical(res, lhs, rhs, flagCy, cur);
        cpuRegs[dst].dw = res;
        break;
    case  0x0F:     //  MVN         Rd = Not Op2
        res = ~rhs;
        flg = setCondLogical(res, lhs, rhs, flagCy, cur);
        cpuRegs[dst].dw = res;
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

#define     ALU_INST_TABLE_REG(OP)                          \
    armALUInstruction<0, OP, 0, ShiftOpLslImm, 0>,          \
    armALUInstruction<0, OP, 0, ShiftOpLslReg, 1>,          \
    armALUInstruction<0, OP, 0, ShiftOpLsrImm, 0>,          \
    armALUInstruction<0, OP, 0, ShiftOpLsrReg, 1>,          \
    armALUInstruction<0, OP, 0, ShiftOpAsrImm, 0>,          \
    armALUInstruction<0, OP, 0, ShiftOpAsrReg, 1>,          \
    armALUInstruction<0, OP, 0, ShiftOpRorImm, 0>,          \
    armALUInstruction<0, OP, 0, ShiftOpRorReg, 1>,          \
    armALUInstruction<0, OP, 1, ShiftOpLslImm, 0>,          \
    armALUInstruction<0, OP, 1, ShiftOpLslReg, 1>,          \
    armALUInstruction<0, OP, 1, ShiftOpLsrImm, 0>,          \
    armALUInstruction<0, OP, 1, ShiftOpLsrReg, 1>,          \
    armALUInstruction<0, OP, 1, ShiftOpAsrImm, 0>,          \
    armALUInstruction<0, OP, 1, ShiftOpAsrReg, 1>,          \
    armALUInstruction<0, OP, 1, ShiftOpRorImm, 0>,          \
    armALUInstruction<0, OP, 1, ShiftOpRorReg, 1>

#define     ALU_INST_TABLE_IMM(OP)                          \
    armALUInstruction<1, OP, 0, ArmALUImmRor, 0>,           \
    armALUInstruction<1, OP, 0, ArmALUImmRor, 0>,           \
    armALUInstruction<1, OP, 0, ArmALUImmRor, 0>,           \
    armALUInstruction<1, OP, 0, ArmALUImmRor, 0>,           \
    armALUInstruction<1, OP, 0, ArmALUImmRor, 0>,           \
    armALUInstruction<1, OP, 0, ArmALUImmRor, 0>,           \
    armALUInstruction<1, OP, 0, ArmALUImmRor, 0>,           \
    armALUInstruction<1, OP, 0, ArmALUImmRor, 0>,           \
    armALUInstruction<1, OP, 1, ArmALUImmRor, 0>,           \
    armALUInstruction<1, OP, 1, ArmALUImmRor, 0>,           \
    armALUInstruction<1, OP, 1, ArmALUImmRor, 0>,           \
    armALUInstruction<1, OP, 1, ArmALUImmRor, 0>,           \
    armALUInstruction<1, OP, 1, ArmALUImmRor, 0>,           \
    armALUInstruction<1, OP, 1, ArmALUImmRor, 0>,           \
    armALUInstruction<1, OP, 1, ArmALUImmRor, 0>,           \
    armALUInstruction<1, OP, 1, ArmALUImmRor, 0>

//  演算の種類 OP (bit24-21) が 08-0b の時、        //
//  つまり TST, TEQ, CMP, CMN 命令の時は、          //
//  演算の結果、フラグをセットしないといけない。    //
//  よってビット 20 をセットしないといけない。      //
//  そうでないビット列は、別の命令に解釈される。    //

#define     ALU_TEST_INST_TABLE_REG(OP)                     \
    nullptr,    nullptr,    nullptr,    nullptr,            \
    nullptr,    nullptr,    nullptr,    nullptr,            \
    armALUInstruction<0, OP, 1, ShiftOpLslImm, 0>,          \
    armALUInstruction<0, OP, 1, ShiftOpLslReg, 1>,          \
    armALUInstruction<0, OP, 1, ShiftOpLsrImm, 0>,          \
    armALUInstruction<0, OP, 1, ShiftOpLsrReg, 1>,          \
    armALUInstruction<0, OP, 1, ShiftOpAsrImm, 0>,          \
    armALUInstruction<0, OP, 1, ShiftOpAsrReg, 1>,          \
    armALUInstruction<0, OP, 1, ShiftOpRorImm, 0>,          \
    armALUInstruction<0, OP, 1, ShiftOpRorReg, 1>

#define     ALU_TEST_INST_TABLE_IMM(OP)                     \
    nullptr,    nullptr,    nullptr,    nullptr,            \
    nullptr,    nullptr,    nullptr,    nullptr,            \
    armALUInstruction<1, OP, 1, ArmALUImmRor, 0>,           \
    armALUInstruction<1, OP, 1, ArmALUImmRor, 0>,           \
    armALUInstruction<1, OP, 1, ArmALUImmRor, 0>,           \
    armALUInstruction<1, OP, 1, ArmALUImmRor, 0>,           \
    armALUInstruction<1, OP, 1, ArmALUImmRor, 0>,           \
    armALUInstruction<1, OP, 1, ArmALUImmRor, 0>,           \
    armALUInstruction<1, OP, 1, ArmALUImmRor, 0>,           \
    armALUInstruction<1, OP, 1, ArmALUImmRor, 0>

CONSTEXPR_VAR   FnALUInst
g_armALUInstTable[512] = {
    ALU_INST_TABLE_REG(0x00),
    ALU_INST_TABLE_REG(0x01),
    ALU_INST_TABLE_REG(0x02),
    ALU_INST_TABLE_REG(0x03),
    ALU_INST_TABLE_REG(0x04),
    ALU_INST_TABLE_REG(0x05),
    ALU_INST_TABLE_REG(0x06),
    ALU_INST_TABLE_REG(0x07),
    ALU_TEST_INST_TABLE_REG(0x08),
    ALU_TEST_INST_TABLE_REG(0x09),
    ALU_TEST_INST_TABLE_REG(0x0A),
    ALU_TEST_INST_TABLE_REG(0x0B),
    ALU_INST_TABLE_REG(0x0C),
    ALU_INST_TABLE_REG(0x0D),
    ALU_INST_TABLE_REG(0x0E),
    ALU_INST_TABLE_REG(0x0F),

    ALU_INST_TABLE_IMM(0x00),
    ALU_INST_TABLE_IMM(0x01),
    ALU_INST_TABLE_IMM(0x02),
    ALU_INST_TABLE_IMM(0x03),
    ALU_INST_TABLE_IMM(0x04),
    ALU_INST_TABLE_IMM(0x05),
    ALU_INST_TABLE_IMM(0x06),
    ALU_INST_TABLE_IMM(0x07),
    ALU_TEST_INST_TABLE_IMM(0x08),
    ALU_TEST_INST_TABLE_IMM(0x09),
    ALU_TEST_INST_TABLE_IMM(0x0A),
    ALU_TEST_INST_TABLE_IMM(0x0B),
    ALU_INST_TABLE_IMM(0x0C),
    ALU_INST_TABLE_IMM(0x0D),
    ALU_INST_TABLE_IMM(0x0E),
    ALU_INST_TABLE_IMM(0x0F),
};

#undef  ALU_INST_TABLE_REG
#undef  ALU_TEST_INST_TABLE_REG
#undef  ALU_INST_TABLE_IMM
#undef  ALU_TEST_INST_TABLE_IMM

}   //  End of (Unnamed) namespace.


//========================================================================
//
//    ArmALU  Instructions.
//

GBD_REGPARM     InstExecResult
CpuArm::execALUInstruction(
        const  OpeCode  opeCode)
{
    //  オペコードから下記のビットを取り出す。          //
    //  bit     25  第二オペランドがレジスタか即値か。  //
    //  bit 24--21  演算の種類 (0x00--0x0f) 。          //
    //  bit     20  フラグレジスタを更新するか否か。    //
    //  bit  6-- 5  シフトの種類 (LSL, LSR, ASR, ROR)   //
    //  bit      4  シフト量指定がレジスタか即値か。    //
    const  OpeCode  idx =
        ((opeCode >> 17) & 0x01F8) | ((opeCode >> 4) & 0x07);
    FnALUInst   pfInst  = g_armALUInstTable[idx];

    char    buf[512];
    sprintf(buf,
            "opeCode = %08x, idx = %03x, pfInst = %p\n",
            opeCode, idx, pfInst);
    std::cerr   <<  buf;

    return  (* pfInst)(opeCode, this->m_cpuRegs, this->m_cpuRegs[16].dw);
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
