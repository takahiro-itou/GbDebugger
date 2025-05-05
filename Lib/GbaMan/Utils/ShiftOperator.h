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

template  <typename T>
CONSTEXPR_FUNC  inline  bool
sobaseLslFlg(
        const  T    value,
        const  int  shift)
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

template  <typename T>
CONSTEXPR_FUNC  inline  bool
sobaseLsrFlg(
        const  T    value,
        const  int  shift)
{
    return ( (value >> (shift - 1)) & 1 ? true : false );
}

//----------------------------------------------------------------
/**   ROR 演算のキャリーフラグ。
**
**/

template  <typename T>
CONSTEXPR_FUNC  inline  bool
sobaseRorFlg(
        const  T    value,
        const  int  shift)
{
    //  キャリーには下から shift - 1  ビット目が入る。  //
    return ( (value >> (shift - 1)) & 1 ? true : false );
}

//----------------------------------------------------------------
/**   ROR 演算の実行結果の値。
**
**/

CONSTEXPR_FUNC  inline  RegType
sobaseRorVal(
        const  RegType  value,
        const  int      shift)
{
    //  ビット [shift-1..0] を [31..(32-shift)] へ移動し、  //
    //  ビット [31..shift]  を [31-shift..0]  へ移動する。  //
    return ( (value << (32 - shift)) | (value >> shift) );
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

struct  ShiftOpLslReg
{
    RegType
    operator()(
            const  RegType  value,
            const  int      shift,
            bool          & flagCy)  const
    {
        if ( LIKELY(shift) ) {
            return  socheckLsl(value, shift, flagCy);
        }
        return ( value );
    }
};

//========================================================================
/**
**    シフト量を即値で指定する LSL
**
**/

struct  ShiftOpLslImm
{
    RegType
    operator()(
            const  RegType  value,
            const  int      shift,
            bool          & flagCy)  const
    {
        if ( UNLIKELY(shift) ) {
            flagCy  = sobaseLslFlg(value, shift);
            return ( value << shift );
        }
        //  LSL#0 はシフトを行わない。  //
        return ( value );
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

struct  ShiftOpLsrImm
{
    RegType
    operator()(
            const  RegType  value,
            const  int      shift,
            bool          & flagCy)  const
    {
        if ( LIKELY(shift) ) {
            flagCy  = sobaseLsrFlg(value, shift);
            return ( value >> shift );
        }
        //  LSR#0 は LSR#32 として解釈される。  //
        flagCy  = (value & 0x80000000) ? true : false;
        return ( value );
    }
};

//========================================================================
/**
**    シフト量をレジスタで指定する ASR
**/

struct  ArmALURmAsrReg
{
    RegType
    operator()(
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

//========================================================================
/**
**    シフト量を即値で指定する ASR
**/

struct  ShiftOpAsrImm
{
    RegType
    operator()(
            const  RegType  value,
            const  int      shift,
            bool          & flagCy)  const
    {
        RegType rhs;
        if ( LIKELY(shift) ) {
            int32_t v = static_cast<int32_t>(value);
            flagCy  = (v >> (int)(shift - 1)) & 1 ? true : false;
            rhs     = v >> (int)(shift);
        } else {
            //  ASR#0 は ASR#32 として解釈される。  //
            if ( value & 0x80000000 ) {
                flagCy  = true;
                rhs     = 0xFFFFFFFF;
            } else {
                flagCy  = false;
                rhs     = 0;
            }
        }
        return ( rhs );
    }
};

//========================================================================
/**
**    シフト量をレジスタで指定する ROR
**/

struct  ArmALURmRorReg
{
    RegType
    operator()(
            const  int      shift,
            const  RegType  vRm,
            bool          & fout_cy,
            const  bool     flag_cy)
    {
        RegType rhs = vRm;
        if ( LIKELY(shift & 0x1F) ) {
            fout_cy = sobaseRorFlg(vRm, shift);
            rhs     = sobaseRorVal(vRm, shift);
        } else {
            if ( shift ) {
                fout_cy = (vRm & 0x80000000 ? true : false);
            }
            rhs = vRm;
        }
        return ( rhs );
    }
};

//========================================================================
/**
**    シフト量を即値で指定する ROR
**/

struct  ShiftOpRorImm
{
    RegType
    operator()(
            const  RegType  vRm,
            const  int      shift,
            bool          & fout_cy,
            const  bool     flag_cy)  const
    {
        RegType rhs;
        if ( LIKELY(shift) ) {
            fout_cy = sobaseRorFlg(vRm, shift);
            rhs     = sobaseRorVal(vRm, shift);
        } else {
            //  ROR#0 は RCR#1  として解釈される。  //
            fout_cy = (vRm & 1) ? true : false;
            rhs     = ((vRm >> 1) | (flag_cy << 31));
        }
        return ( rhs );
    }
};

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END

#endif
