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
**      An Implementation of ShiftOperators.
**
**      @file       GbaMan/Utils/ShiftOperator.h
**/

#if !defined( GBDEBUGGER_GBAMAN_UTILS_INCLUDED_SHIFT_OPERATOR_H )
#    define   GBDEBUGGER_GBAMAN_UTILS_INCLUDED_SHIFT_OPERATOR_H

#if !defined( GBDEBUGGER_GBAMAN_INCLUDED_CPU_UTILS_H )
#    include    "GbDebugger/GbaMan/CpuUtils.h"
#endif


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

//  クラスの前方宣言。  //

//========================================================================
//
//    ビットシフト演算の基本実装。
//

//----------------------------------------------------------------
/**   LSL 演算のキャリーフラグ
**
**  @param [in] value
**  @param [in] shift   シフト量 (1-32)
**/

CONSTEXPR_FUNC  inline  bool
sobaseLslFlg(
        const  RegType  value,
        const  int      shift)
{
    return ( (value >> (32 - shift)) & 1 ? true : false );
}

//----------------------------------------------------------------
/**   LSL 演算の実行結果の値。
**
**/

CONSTEXPR_FUNC  inline  RegType
sobaseLslVal(
        const  RegType  value,
        const  int      shift)
{
    return ( value << shift );
}

//----------------------------------------------------------------
/**   LSR 演算のキャリーフラグ。
**
**/

CONSTEXPR_FUNC  inline  bool
sobaseLsrFlg(
        const  RegType  value,
        const  int      shift)
{
    return ( (value >> (shift - 1)) & 1 ? true : false );
}

//----------------------------------------------------------------

inline  RegType
socheckLsl(
        const  RegType  value,
        const  int      shift,
        bool          & outflgC)
{
    RegType retVal  = value;
    if ( shift == 32 ) {
        //  シフト量がレジスタの幅に等しいので結果は必ず０  //
        //  ただし元の最下位ビットがキャリーに入る。        //
        outflgC = (value & 1 ? true : false);
        retVal  = 0;
    } else if ( LIKELY(shift < 32) ) {
        outflgC = sobaseLslFlg(value, shift);
        retVal  = (value << shift);
    } else {
        //  シフト量がレジスタの幅を超えているので結果は必ず０  //
        outflgC = 0;
        retVal  = 0;
    }
    return ( retVal );
}

//========================================================================
/**
**    シフト量をレジスタで指定する LSL
**/

struct  ArmALURmLslReg
{
    RegType
    operator()(
            const  int      shift,
            const  RegType  vRm,
            bool          & fout_cy,
            const  bool     flag_cy)  const
    {
        if ( LIKELY(shift) ) {
            return  socheckLsl(vRm, shift, fout_cy);
        }
        return ( vRm );
    }
};

//========================================================================
/**
**    シフト量を即値で指定する LSL
**/

struct  ArmALURmLslImm
{
    RegType
    operator()(
            const  int      shift,
            const  RegType  vRm,
            bool          & fout_cy,
            const  bool     flag_cy)  const
    {
        RegType rhs = vRm;
        if ( UNLIKELY(!shift) ) {
            fout_cy = sobaseLslFlg(vRm, shift);
            rhs     <<= shift;
        }
        //  LSL#0 はシフトを行わない。  //

        return ( rhs );
    }
};

//========================================================================
/**
**    シフト量をレジスタで指定する LSR
**/

struct  ArmALURmLsrReg
{
    RegType
    operator()(
            const  int      shift,
            const  RegType  vRm,
            bool          & fout_cy,
            const  bool     flag_cy)  const
    {
        RegType rhs = vRm;
        if ( LIKELY(shift) ) {
            if ( shift == 32 ) {
                fout_cy = (vRm & 0x80000000) ? true : false;
                rhs     = 0;
            } else if ( LIKELY(shift < 32) ) {
                fout_cy = sobaseLsrFlg(vRm, shift);
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

//========================================================================
/**
**    シフト量を即値で指定する LSR
**/

struct  ArmALURmLsrImm
{
    RegType
    operator()(
            const  int      shift,
            const  RegType  vRm,
            bool          & fout_cy,
            const  bool     flag_cy)  const
    {
        RegType rhs = vRm;
        if ( LIKELY(shift) ) {
            fout_cy = sobaseLsrFlg(vRm, shift);
            rhs     >>= shift;
        } else {
            //  LSR#0 は LSR#32 として解釈される。  //
            fout_cy = (vRm & 0x80000000) ? true : false;
            rhs     = 0;
        }
        return ( rhs );
    }
};

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END

#endif
