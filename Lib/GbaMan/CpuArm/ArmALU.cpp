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

struct  ArmALURmLslReg
{
    RegType operator()(
            const  int      shift,
            const  RegType  vRm,
            bool          & fout_cy,
            const  bool     flag_cy)
    {
        RegType rhs = vRm;
        if ( LIKELY(shift) ) {
            if ( shift == 32 ) {
                fout_cy = (vRm & 1 ? true : false);
                rhs     = 0;
            } else if ( LIKELY(shift < 32) ) {
                fout_cy = (vRm >> (32 - shift)) & 1 ? true : false;
                rhs     = (vRm << shift);
            } else {
                fout_cy = false;
                rhs     = 0;
            }
        }
        return ( rhs );
    }
};

struct  ArmALURmLslImm
{
    RegType operator()(
            const  int      shift,
            const  RegType  vRm,
            bool          & fout_cy,
            const  bool     flag_cy)
    {
        RegType rhs = vRm;
        if ( UNLIKELY(!shift) ) {
            fout_cy = (vRm >> (32 - shift)) & 1 ? true : false;
            rhs     <<= shift;
        }
        //  LSL#0 はシフトを行わない。  //

        return ( rhs );
    }
};

struct  ArmALURmLsrReg
{
    RegType operator()(
            const  int      shift,
            const  RegType  vRm,
            bool          & fout_cy,
            const  bool     flag_cy)
    {
        RegType rhs = vRm;
        if ( LIKELY(shift) ) {
            if ( shift == 32 ) {
                fout_cy = (vRm & 0x80000000) ? true : false;
                rhs     = 0;
            } else if ( LIKELY(shift < 32) ) {
                fout_cy = (vRm >> (shift - 1)) & 1 ? true : false;
                rhs     = (vRm >> shift);
            } else {
                fout_cy = false;
                rhs     = 0;
            }
        } else {
            rhs = vRm;
        }
        return ( rhs );
    }
};

struct  ArmALURmLsrImm
{
    RegType operator()(
            const  int      shift,
            const  RegType  vRm,
            bool          & fout_cy,
            const  bool     flag_cy)
    {
        RegType rhs = vRm;
        if ( LIKELY(shift) ) {
            fout_cy = (vRm >> (shift - 1)) & 1 ? true : false;
            rhs     >>= shift;
        } else {
            //  LSR#0 は LSR#32 として解釈される。  //
            fout_cy = (vRm & 0x80000000) ? true : false;
            rhs     = 0;
        }
        return ( rhs );
    }
};

struct  ArmALURmAsrReg
{
    RegType operator()(
            const  int      shift,
            const  RegType  vRm,
            bool          & fout_cy,
            const  bool     flag_cy)
    {
        RegType rhs = vRm;
        if ( LIKELY(shift < 32) ) {
            if ( LIKELY(shift) ) {
                int32_t v = static_cast<int32_t>(vRm);
                fout_cy = (v >> (int)(shift - 1)) & 1 ? true : false;
                rhs     = v >> (int)(shift);
            } else {
                rhs     = vRm;
            }
        } else {
            if ( vRm & 0x80000000 ) {
                fout_cy = true;
                rhs     = 0xFFFFFFFF;
            } else {
                fout_cy = false;
                rhs     = 0;
            }
        }
        return ( rhs );
    }
};

struct  ArmALURmAsrImm
{
    RegType operator()(
            const  int      shift,
            const  RegType  vRm,
            bool          & fout_cy,
            const  bool     flag_cy)
    {
        RegType rhs;
        if ( LIKELY(shift) ) {
            int32_t v = static_cast<int32_t>(vRm);
            fout_cy = (v >> (int)(shift - 1)) & 1 ? true : false;
            rhs     = v >> (int)(shift);
        } else {
            //  ASR#0 は ASR#32 として解釈される。  //
            if ( vRm & 0x80000000 ) {
                fout_cy = true;
                rhs     = 0xFFFFFFFF;
            } else {
                fout_cy = false;
                rhs     = 0;
            }
        }
        return ( rhs );
    }
};

struct  ArmALURmRorReg
{
    RegType operator()(
            const  int      shift,
            const  RegType  vRm,
            bool          & fout_cy,
            const  bool     flag_cy)
    {
        RegType rhs = vRm;
        if ( LIKELY(shift & 0x1F) ) {
            //fout_cy = (vRm >> (shift - 1)) & 1 ? true : false;
            fout_cy = armRorFlg(vRm, shift);
            rhs     = armRorVal(vRm, shift);
            //rhs = ((vRm < (32 - shift)) | (vRm >> shift));
        } else {
            if ( shift ) {
                fout_cy = (vRm & 0x80000000 ? true : false);
            }
            rhs = vRm;
        }
        return ( rhs );
    }
};

struct  ArmALURmRorImm
{
    RegType operator()(
            const  int      shift,
            const  RegType  vRm,
            bool          & fout_cy,
            const  bool     flag_cy)
    {
        RegType rhs;
        if ( LIKELY(shift) ) {
//            fout_cy = (vRm >> (shift - 1)) & 1 ? true : false;
            fout_cy = armRorFlg(vRm, shift);
            rhs     = armRorVal(vRm, shift);   //((vRm << (32 - shift)) | (vRm >> shift));
        } else {
            //  ROR#0 は RCR#1  として解釈される。  //
            fout_cy = (vRm & 1) ? true : false;
            rhs     = ((vRm >> 1) | (flag_cy << 31));
        }
        return ( rhs );
    }
};

struct  ArmALUImmRor
{
    RegType operator()(
            const  int      shift,
            const  RegType  vImm,
            bool          & fout_cy,
            const  bool     flag_cy)
    {
        RegType rhs;
        if ( UNLIKELY(shift) ) {
            fout_cy = armRorFlg(vImm, shift);
            rhs     = armRorVal(vImm, shift);
        }
        return ( rhs );
    }
};

template  <int  BIT25, int CODE, int BIT20, int SHIFTTYPE, int BIT4>
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
    std::cerr   <<  __PRETTY_FUNCTION__;
#endif
    sprintf(buf,
            "Op2 = %d, CODE = %x, S = %d, SHIFT = %d, BIT4 = %d\n",
            BIT25, CODE, BIT20, SHIFTTYPE, BIT4);
    std::cerr   <<  buf;
    sprintf(buf,
            "OpeCode = %08x, dst = %d\n",
            dst);

    //  第一オペランドレジスタはビット 16..19 で指定。  //
    const  RegType  lhs = cpuRegs[(opeCode >> 16) & 0x0F].dw;
    RegType         rhs;
    uint64_t        res;

    const   RegType Cy      = (cpuFlag >> 29) & 0x01;
    const   bool    flag_cy = (cpuFlag & 0x20000000) ? true : false;
    bool            fout_cy = flag_cy;

    if ( BIT25 == 0 ) {
        //  第二オペランドはレジスタ。ビット 00..07 で指定される。  //
        OpeCode iRm = (opeCode & 0x0F);
        RegType vRm = cpuRegs[iRm].dw;

        if ( BIT4 == 0 ) {
            //  シフト量の指定は即値。ビット 07..11 で指定。    //
            const int shift = (opeCode >> 7) & 0x1F;

            //  ビット 05..06 はシフトの種類。  //
            switch ( SHIFTTYPE ) {
            case  0:    //  LSL
                rhs = ArmALURmLslImm()(shift, vRm, fout_cy, flag_cy);
                break;
            case  1:    //  LSR
                rhs = ArmALURmLsrImm()(shift, vRm, fout_cy, flag_cy);
                break;
            case  2:    //  ASR
                rhs = ArmALURmAsrImm()(shift, vRm, fout_cy, flag_cy);
                break;
            case  3:    //  ROR
                rhs = ArmALURmRorImm()(shift, vRm, fout_cy, flag_cy);
                break;
            }
        } else {
            //  シフト量指定はレジスタ。ビット 08..11 で指定。  //
            const int shift = cpuRegs[(opeCode >> 8) & 0x0F].dw;
            if ( iRm == 15 ) {
                vRm += 4;       //  オペランド Rm が R15 (PC) の時  //
            }

            //  ビット 05..06 はシフトの種類。  //
            switch ( SHIFTTYPE ) {
            case  0:    //  LSL
                rhs = ArmALURmLslReg()(shift, vRm, fout_cy, flag_cy);
                break;
            case  1:    //  LSR
                rhs = ArmALURmLsrReg()(shift, vRm, fout_cy, flag_cy);
                break;
            case  2:    //  ASR
                rhs = ArmALURmAsrReg()(shift, vRm, fout_cy, flag_cy);
                break;
            case  3:    //  ROR
                rhs = ArmALURmRorReg()(shift, vRm, fout_cy, flag_cy);
                break;
            }
        }
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
