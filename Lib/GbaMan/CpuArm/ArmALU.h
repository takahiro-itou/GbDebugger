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
**      An Interface of ArmALU class.
**
**      @file       GbaMan/ArmALU.h
**/

#if !defined( GBDEBUGGER_GBAMAN_INCLUDED_ARM_ALU_H )
#    define   GBDEBUGGER_GBAMAN_INCLUDED_ARM_ALU_H

#if !defined( GBDEBUGGER_GBAMAN_INCLUDED_CPU_UTILS_H )
#    include    "GbDebugger/GbaMan/CpuUtils.h"
#endif

#if !defined( GBDEBUGGER_COMMON_INCLUDED_DEBUGGER_UTILS_H )
#    include    "GbDebugger/Common/DebuggerUtils.h"
#endif

#include    "../Utils/ShiftOperator.h"


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

//  クラスの前方宣言。  //
class   MemoryManager;


//========================================================================
//
//    ArmALU  Instructions.
//

GBD_REGPARM     InstExecResult
armALUInstruction(
        const  OpeCode  opeCode,
        RegPair         cpuRegs[],
        uint32_t      & cpuFlag);

//========================================================================
//
//    第二オペランドの指定に使うファンクタ。
//


struct  ArmALUImmRor
{
    RegType
    operator()(
            const  int      shift,
            const  RegType  vImm,
            bool          & fout_cy,
            const  bool     flag_cy)
    {
        RegType rhs = vImm;
        if ( UNLIKELY(shift) ) {
            fout_cy = armRorFlg(vImm, shift);
            rhs     = armRorVal(vImm, shift);
        }
        return ( rhs );
    }
};

inline  const   RegType
armImmRor(
        const  int      shift,
        const  RegType  vImm)
{
    if ( UNLIKELY(shift) ) {
        return  armRorVal(vImm, shift);
    }
    return ( vImm );
}

//========================================================================
//
//    第二オペランドを決定する。
//

template <int SHIFTTYPE, int BIT4>
inline  const   RegType
getAluOp2Register(
        const  OpeCode  opeCode,
        const  RegPair  cpuRegs[],
        bool          & fout_cy,
        const  bool     flag_cy)
{
    RegType rhs;

    //  第二オペランドはレジスタ。ビット 00..07 で指定される。  //
    OpeCode iRm = (opeCode & 0x0F);
    RegType vRm = cpuRegs[iRm].dw;

    if ( BIT4 == 0 ) {
        //  シフト量の指定は即値。ビット 07..11 で指定。    //
        const int shift = (opeCode >> 7) & 0x1F;

        //  ビット 05..06 はシフトの種類。  //
        switch ( SHIFTTYPE ) {
        case  0:    //  LSL
            rhs = ShiftOpLslImm()(vRm, shift, fout_cy);
            break;
        case  1:    //  LSR
            rhs = ShiftOpLsrImm()(vRm, shift, fout_cy);
            break;
        case  2:    //  ASR
            rhs = ShiftOpAsrImm()(vRm, shift, fout_cy);
            break;
        case  3:    //  ROR
            rhs = ShiftOpRorImm()(vRm, shift, fout_cy, flag_cy);
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
            rhs = ShiftOpLslReg()(vRm, shift, fout_cy);
            break;
        case  1:    //  LSR
            rhs = ShiftOpLsrReg()(vRm, shift, fout_cy);
            break;
        case  2:    //  ASR
            rhs = ShiftOpAsrReg()(vRm, shift, fout_cy);
            break;
        case  3:    //  ROR
            rhs = ShiftOpRorReg()(vRm, shift, fout_cy);
            break;
        }
    }

    return ( rhs );
}

class  ArmALU
{

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

//========================================================================
//
//    Member Variables.
//

//========================================================================
//
//    Other Features.
//
private:
    typedef     ArmALU          This;
    ArmALU              (const  This  &);
    This &  operator =  (const  This  &);
public:
    //  テストクラス。  //
    friend  class   ArmALUTest;
};

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END

#endif
