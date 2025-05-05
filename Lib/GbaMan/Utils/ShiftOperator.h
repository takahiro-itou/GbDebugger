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

struct  ShiftOpLsrReg
{
    RegType
    operator()(
            const  RegType  value,
            const  int      shift,
            bool          & flagCy)  const
    {
        RegType rhs = value;
        if ( LIKELY(shift) ) {
            if ( shift == 32 ) {
                flagCy  = (value & 0x80000000) ? true : false;
                rhs     = 0;
            } else if ( LIKELY(shift < 32) ) {
                flagCy  = sobaseLsrFlg(value, shift);
                rhs     = (value >> shift);
            } else {
                flagCy  = false;
                rhs     = 0;
            }
        } else {
            rhs = value;
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

struct  ShiftOpAsrReg
{
    RegType
    operator()(
            const  RegType  value,
            const  int      shift,
            bool          & flagCy)  const
    {
        RegType rhs = value;
        if ( LIKELY(shift < 32) ) {
            if ( LIKELY(shift) ) {
                int32_t v = static_cast<int32_t>(value);
                flagCy  = (v >> (int)(shift - 1)) & 1 ? true : false;
                rhs     = v >> (int)(shift);
            } else {
                rhs     = value;
            }
        } else {
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

struct  ShiftOpRorReg
{
    RegType
    operator()(
            const  RegType  value,
            const  int      shift,
            bool          & flagCy)  const
    {
        RegType rhs = value;
        if ( LIKELY(shift & 0x1F) ) {
            flagCy  = sobaseRorFlg(value, shift);
            rhs     = sobaseRorVal(value, shift);
        } else {
            if ( shift ) {
                flagCy  = (value & 0x80000000 ? true : false);
            }
            rhs = value;
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
            const  RegType  value,
            const  int      shift,
            bool          & flagCy,
            const  bool     flag_cy)  const
    {
        RegType rhs;
        if ( LIKELY(shift) ) {
            flagCy  = sobaseRorFlg(value, shift);
            rhs     = sobaseRorVal(value, shift);
        } else {
            //  ROR#0 は RCR#1  として解釈される。  //
            const  RegType  msb = (flagCy << 31);
            flagCy  = (value & 1) ? true : false;
            rhs     = ((value >> 1) | (msb));
        }
        return ( rhs );
    }
};

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END

#endif
