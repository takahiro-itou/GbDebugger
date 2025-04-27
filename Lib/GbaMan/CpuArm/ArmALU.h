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

struct  ArmALURmLslReg
{
    RegType
    operator()(
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
    RegType
    operator()(
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
    RegType
    operator()(
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
    RegType
    operator()(
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

struct  ArmALURmAsrImm
{
    RegType
    operator()(
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
    RegType
    operator()(
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
    RegType
    operator()(
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
