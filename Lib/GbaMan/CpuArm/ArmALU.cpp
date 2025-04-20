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


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

namespace  {

typedef     GBD_REGPARM     InstExecResult
(* FnALUInst)(
        const  OpeCode  opeCode,
        RegPair         cpuRegs[],
        RegType       & cpuFlag);

template  <int  BIT25, int CODE, int BIT20, int SHIFTTYPE, int BIT4>
GBD_REGPARM     InstExecResult
armALUInstruction(
        const  OpeCode  opeCode,
        RegPair         cpuRegs[],
        RegType       & cpuFlag)
{
    //  結果を格納するレジスタはビット 12..15 で指定。  //
    const  int      dst = (opeCode >> 12) & 0x0F;

    //  第一オペランドレジスタはビット 16..19 で指定。  //
    const  RegType  lhs = cpuRegs[(opeCode >> 16) & 0x0F].dw;
    RegType         rhs;

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
            case  1:    //  LSR
            case  2:    //  ASR
            case  3:    //  ROR
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
                if ( LIKELY(shift) ) {
                    if ( shift == 32 ) {
                        rhs     = 0;
                        fout_cy = (vRm & 1 ? true : false);
                    } else if ( LIKELY(shift < 32) ) {
                        fout_cy = (vRm >> (32 - shift)) & 1 ? true : false;
                        rhs     = (vRm << shift);
                    } else {
                        rhs     = 0;
                        fout_cy = false;
                    }
                } else {
                    rhs = vRm;
                }
                break;
            case  1:    //  LSR
                if ( LIKELY(shift) ) {
                    if ( shift == 32 ) {
                        rhs     = 0;
                        fout_cy = (vRm & 0x80000000) ? true : false;
                    } else if ( LIKELY(shift < 32) ) {
                        fout_cy = (vRm >> (shift - 1)) & 1 ? true : false;
                        rhs     = (vRm >> shift);
                    } else {
                        rhs     = 0;
                        fout_cy = false;
                    }
                } else {
                    rhs = vRm;
                }
                break;
            case  2:    //  ASR
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
                        rhs     = 0xFFFFFFFF;
                        fout_cy = true;
                    } else {
                        rhs     = 0;
                        fout_cy = false;
                    }
                }
                break;
            case  3:    //  ROR
                if ( LIKELY(shift & 0x1F) ) {
                    fout_cy = (vRm >> (shift - 1)) & 1 ? true : false;
                    rhs = ((vRm < (32 - shift)) | (vRm >> shift));
                } else {
                    if ( shift ) {
                        fout_cy = (vRm & 0x80000000 ? true : false);
                    }
                    rhs = vRm;
                }
                break;
            }
        }
    } else {
        //  第二オペランドは即値指定。ビット 00..07 で指定される。  //
        const  RegType  imm = (opeCode & 0xFF);
        const  int      ror = (opeCode & 0xF00) >> 7;
        rhs = ((imm << (32 - ror)) | (imm >> ror));
    }

    if ( BIT20 == 0 ) {
        //  フラグレジスタを更新する。  //
    }

    return ( InstExecResult::SUCCESS_CONTINUE );
}

CONSTEXPR_VAR   FnALUInst
g_armALUInstTable[512] = {
    armALUInstruction<0, 0, 0, 0, 0>
};

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
