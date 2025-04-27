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

template  <int BIT25, int CODE, int BIT20, int SHIFTTYPE, int BIT4>
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
            "Op2(I/R) = %d, CODE = %x, S = %d, SHIFT = %d, BIT4(R) = %d\n",
            BIT25, CODE, BIT20, SHIFTTYPE, BIT4);
    std::cerr   <<  buf;
    sprintf(buf,
            "OpeCode = %08x, dst = %d\n",
            opeCode, dst);
    std::cerr   <<  buf;

    //  第一オペランドレジスタはビット 16..19 で指定。  //
    const  RegType  lhs = cpuRegs[(opeCode >> 16) & 0x0F].dw;
    RegType         rhs;
    uint64_t        res;

    const   RegType Cy      = (cpuFlag >> 29) & 0x01;
    const   bool    flag_cy = (cpuFlag & CPSR::FLAG_C) ? true : false;
    bool            fout_cy = flag_cy;

    if ( BIT25 == 0 ) {
        //  第二オペランドはレジスタ。ビット 00..07 で指定される。  //
        rhs = getAluOp2Register<SHIFTTYPE, BIT4>(
                opeCode, cpuRegs, fout_cy, flag_cy);
    } else {
        //  第二オペランドは即値指定。ビット 00..07 で指定される。  //
        const  RegType  imm = (opeCode & 0xFF);
        const  int      ror = (opeCode & 0xF00) >> 7;
        rhs = ArmALUImmRor()(ror, imm, fout_cy, flag_cy);
    }

    const  RegType  cur = cpuFlag;
    RegType         flg;

    switch ( CODE ) {
    case  0x00:     //  AND         Rd = Rn AND Op2
        res = lhs & rhs;
        flg = setCondLogical(res, lhs, rhs, fout_cy, cur);
        cpuRegs[dst].dw = res;
        break;
    case  0x01:     //  EOR (XOR)   Rd = Rn XOR Op2
        res = lhs ^ rhs;
        flg = setCondLogical(res, lhs, rhs, fout_cy, cur);
        cpuRegs[dst].dw = res;
        break;
    case  0x02:     //  SUB         Rd = Rn - OP2
        res = static_cast<uint64_t>(lhs) - static_cast<uint64_t>(rhs);
        flg = setCondSub(res, lhs, rhs, fout_cy, cur);
        cpuRegs[dst].dw = res;
        break;
    case  0x03:     //  RSB         Rd = Op2 - Rn
        res = static_cast<uint64_t>(rhs) - static_cast<uint64_t>(lhs);
        flg = setCondSub(res, rhs, lhs, fout_cy, cur);
        cpuRegs[dst].dw = res;
        break;
    case  0x04:     //  ADD         Rd = Rn + Op2
        res = static_cast<uint64_t>(lhs) + static_cast<uint64_t>(rhs);
        flg = setCondAdd(res, lhs, rhs, fout_cy, cur);
        cpuRegs[dst].dw = res;
        break;
    case  0x05:     //  ADC         Rd = Rn + Op2 + Cy
        res = static_cast<uint64_t>(lhs) + static_cast<uint64_t>(rhs)
                    + static_cast<uint64_t>(Cy);
        flg = setCondAdd(res, lhs, rhs, fout_cy, cur);
        cpuRegs[dst].dw = res;
        break;
    case  0x06:     //  SBC         Rd = Rn - Op2 + Cy - 1
        res = static_cast<uint64_t>(lhs) - static_cast<uint64_t>(rhs)
                    + static_cast<uint64_t>(Cy - 1);
        flg = setCondAdd(res, lhs, rhs, fout_cy, cur);
        cpuRegs[dst].dw = res;
        break;
    case  0x07:     //  RSC         Rd = Op2 - Rn + Cy - 1
        res = static_cast<uint64_t>(rhs) - static_cast<uint64_t>(lhs)
                    + static_cast<uint64_t>(Cy - 1);
        flg = setCondAdd(res, rhs, lhs, fout_cy, cur);
        cpuRegs[dst].dw = res;
        break;
    case  0x08:     //  TST         (void)(Rn AND Op2)
        res = lhs & rhs;
        flg = setCondLogical(res, lhs, rhs, fout_cy, cur);
        break;
    case  0x09:     //  TEQ         (void)(Rn XOR Op2)
        res = lhs ^ rhs;
        flg = setCondLogical(res, lhs, rhs, fout_cy, cur);
        break;
    case  0x0A:     //  CMP         (void)(Rn - Op2)
        res = static_cast<uint64_t>(lhs) - static_cast<uint64_t>(rhs);
        flg = setCondSub(res, lhs, rhs, fout_cy, cur);
        break;
    case  0x0B:     //  CMN         (void)(Rn + Op2)
        res = static_cast<uint64_t>(lhs) + static_cast<uint64_t>(rhs);
        flg = setCondAdd(res, lhs, rhs, fout_cy, cur);
        break;
    case  0x0C:     //  ORR (OR)    Rd = Rn OR Op2
        res = lhs | rhs;
        flg = setCondLogical(res, lhs, rhs, fout_cy, cur);
        cpuRegs[dst].dw = res;
        break;
    case  0x0D:     //  MOV         Rd = Op2
        res = rhs;
        flg = setCondLogical(res, lhs, rhs, fout_cy, cur);
        cpuRegs[dst].dw = res;
        break;
    case  0x0E:     //  BIC         Rd = Rnn AND NOT Op2
        res = lhs & ~rhs;
        flg = setCondLogical(res, lhs, rhs, fout_cy, cur);
        cpuRegs[dst].dw = res;
        break;
    case  0x0F:     //  MVN         Rd = Not Op2
        res = ~rhs;
        flg = setCondLogical(res, lhs, rhs, fout_cy, cur);
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

#define     ARMALU_INST_TABLE(RN2, OP)          \
    armALUInstruction<RN2, OP, 0, 0, 0>,        \
    armALUInstruction<RN2, OP, 0, 0, 1>,        \
    armALUInstruction<RN2, OP, 0, 1, 0>,        \
    armALUInstruction<RN2, OP, 0, 1, 1>,        \
    armALUInstruction<RN2, OP, 0, 2, 0>,        \
    armALUInstruction<RN2, OP, 0, 2, 1>,        \
    armALUInstruction<RN2, OP, 0, 3, 0>,        \
    armALUInstruction<RN2, OP, 0, 3, 1>,        \
    armALUInstruction<RN2, OP, 1, 0, 0>,        \
    armALUInstruction<RN2, OP, 1, 0, 1>,        \
    armALUInstruction<RN2, OP, 1, 1, 0>,        \
    armALUInstruction<RN2, OP, 1, 1, 1>,        \
    armALUInstruction<RN2, OP, 1, 2, 0>,        \
    armALUInstruction<RN2, OP, 1, 2, 1>,        \
    armALUInstruction<RN2, OP, 1, 3, 0>,        \
    armALUInstruction<RN2, OP, 1, 3, 1>

CONSTEXPR_VAR   FnALUInst
g_armALUInstTable[512] = {
    ARMALU_INST_TABLE(0, 0x00),
    ARMALU_INST_TABLE(0, 0x01),
    ARMALU_INST_TABLE(0, 0x02),
    ARMALU_INST_TABLE(0, 0x03),
    ARMALU_INST_TABLE(0, 0x04),
    ARMALU_INST_TABLE(0, 0x05),
    ARMALU_INST_TABLE(0, 0x06),
    ARMALU_INST_TABLE(0, 0x07),
    ARMALU_INST_TABLE(0, 0x08),
    ARMALU_INST_TABLE(0, 0x09),
    ARMALU_INST_TABLE(0, 0x0A),
    ARMALU_INST_TABLE(0, 0x0B),
    ARMALU_INST_TABLE(0, 0x0C),
    ARMALU_INST_TABLE(0, 0x0D),
    ARMALU_INST_TABLE(0, 0x0E),
    ARMALU_INST_TABLE(0, 0x0F),

    ARMALU_INST_TABLE(1, 0x00),
    ARMALU_INST_TABLE(1, 0x01),
    ARMALU_INST_TABLE(1, 0x02),
    ARMALU_INST_TABLE(1, 0x03),
    ARMALU_INST_TABLE(1, 0x04),
    ARMALU_INST_TABLE(1, 0x05),
    ARMALU_INST_TABLE(1, 0x06),
    ARMALU_INST_TABLE(1, 0x07),
    ARMALU_INST_TABLE(1, 0x08),
    ARMALU_INST_TABLE(1, 0x09),
    ARMALU_INST_TABLE(1, 0x0A),
    ARMALU_INST_TABLE(1, 0x0B),
    ARMALU_INST_TABLE(1, 0x0C),
    ARMALU_INST_TABLE(1, 0x0D),
    ARMALU_INST_TABLE(1, 0x0E),
    ARMALU_INST_TABLE(1, 0x0F),
};

#undef  ARMALU_INST_TABLE

}   //  End of (Unnamed) namespace.


//========================================================================
//
//    ArmALU  Instructions.
//

GBD_REGPARM     InstExecResult
CpuArm::armALUInstruction(
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
