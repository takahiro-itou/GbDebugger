﻿//  -*-  coding: utf-8-with-signature;  mode: c++  -*-  //
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
**      An Interface of CpuArm class.
**
**      @file       GbaMan/CpuArm.h
**/

#if !defined( GBDEBUGGER_GBAMAN_CPUARM_INCLUDED_CPU_ARM_H )
#    define   GBDEBUGGER_GBAMAN_CPUARM_INCLUDED_CPU_ARM_H

#if !defined( GBDEBUGGER_GBAMAN_INCLUDED_BASE_CPU_CORE_H )
#    include    "GbDebugger/GbaMan/BaseCpuCore.h"
#endif

#if !defined( GBDEBUGGER_COMMON_INCLUDED_DEBUGGER_UTILS_H )
#    include    "GbDebugger/Common/DebuggerUtils.h"
#endif

#if !defined( GBDEBUGGER_GBAMAN_CPUARM_INCLUDED_OPERATOR_CONST_H )
#    include    "OperatorConst.h"
#endif

#if !defined( GBDEBUGGER_GBAMAN_INCLUDED_CPU_UTILS_H )
#    include    "GbDebugger/GbaMan/CpuUtils.h"
#endif

#if !defined( GBDEBUGGER_SYS_STL_INCLUDED_IOSFWD )
#    include    <iosfwd>
#    define   GBDEBUGGER_SYS_STL_INCLUDED_IOSFWD
#endif


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

//========================================================================
//
//    CpuArm  class.
//

class  CpuArm : public BaseCpuCore
{

//========================================================================
//
//    Internal Type Definitions.
//
private:

    typedef     BaseCpuCore     Super;

    typedef     GBD_REGPARM     InstExecResult
    (CpuArm::* FnInst)(
            const  OpeCode  opeCode);

//========================================================================
//
//    Constructor(s) and Destructor.
//
public:

    //----------------------------------------------------------------
    /**   インスタンスを初期化する
    **  （コンストラクタ）。
    **
    **/
    CpuArm(
            GbaManager    & manGba,
            MemoryManager & manMem);

    //----------------------------------------------------------------
    /**   インスタンスを破棄する
    **  （デストラクタ）。
    **
    **/
    virtual  ~CpuArm();

//========================================================================
//
//    Public Member Functions (Implement Pure Virtual).
//
public:

    //----------------------------------------------------------------
    /**   現在の命令を実行する。
    **
    **/
    InstExecResult
    executeNextInst();

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
public:

    //----------------------------------------------------------------
    /**   プログラムカウンタを取得する。
    **
    **/
    const   GuestMemoryAddress
    getNextPC()  const
    {
        return ( this->m_nextPC );
    }

//========================================================================
//
//    Protected Member Functions.
//

//========================================================================
//
//    For Internal Use Only.
//
private:

    //----------------------------------------------------------------
    //    命令の実行を行う関数たち。
    //

#if 0
    GBD_REGPARM     InstExecResult
    execALUInstruction(
            const  OpeCode  opeCode);
#endif

    template  <int BIT25, AluOp CODE, int BIT20, typename SHIFTOP, int BIT4>
    GBD_REGPARM     InstExecResult
    execArithmeticLogic(
            const  OpeCode  opeCode);

    GBD_REGPARM     InstExecResult
    execArm100_MrsCpsr(
            const  OpeCode  opeCode);

    GBD_REGPARM     InstExecResult
    execArm120_MsrCpsrReg(
            const  OpeCode  opeCode);

    GBD_REGPARM     InstExecResult
    execArm121_BX(
            const  OpeCode  opeCode);

    GBD_REGPARM     InstExecResult
    execArm140_MrsSpsr(
            const  OpeCode  opeCode);

    GBD_REGPARM     InstExecResult
    execArm160_MsrSpsrReg(
            const  OpeCode  opeCode);

    GBD_REGPARM     InstExecResult
    execArm320_MsrCpsrImm(
            const  OpeCode  opeCode);

    GBD_REGPARM     InstExecResult
    execArm360_MsrSpsrImm(
            const  OpeCode  opeCode);

    GBD_REGPARM     InstExecResult
    execArmAxx_B(
            const  OpeCode  opeCode);

    GBD_REGPARM     InstExecResult
    execArmBxx_BL(
            const  OpeCode  opeCode);

    template  <MulOp OP>
    GBD_REGPARM     InstExecResult
    execMultiplyHalf(
            const  OpeCode  opeCode);

    template  <MulOp OP, int S>
    GBD_REGPARM     InstExecResult
    execMultiplyWord(
            const  OpeCode  opeCode);

    GBD_REGPARM     InstExecResult
    execNotImplemented(
            const  OpeCode  opeCode);

    template  <int P, int U, int S, int W, int L>
    GBD_REGPARM     InstExecResult
    execOperateStack(
            const  OpeCode  opeCode);

    GBD_REGPARM     InstExecResult
    execSoftwareInterrupt(
            const  OpeCode  opeCode);

    template  <int I, int P, int U, typename B,
               int BIT21, int OP, typename SHIFTOP>
    GBD_REGPARM     InstExecResult
    execStoreLoad(
            const  OpeCode  opeCode);

    template  <int P, int U, int I, int W, MemOp OP, typename B>
    GBD_REGPARM     InstExecResult
    execStoreLoadHalf(
            const  OpeCode  opeCode);

    GBD_REGPARM     InstExecResult
    execUnknownInstruction(
            const  OpeCode  opeCode);

    //----------------------------------------------------------------
    /**   命令を実行する。
    **
    **/
    int
    executeInst(
            const  OpeCode  opeCode);

    //----------------------------------------------------------------
    /**   命令をプリフェッチする。
    **
    **/
    void
    prefetchAll();

    //----------------------------------------------------------------
    /**   次の命令をプリフェッチする。
    **
    **/
    void
    prefetchNext();

//========================================================================
//
//    Member Variables.
//
private:

    /**   命令テーブル。        **/
    static  const   FnInst  s_armInstTable[4096];

//========================================================================
//
//    Other Features.
//
private:
    typedef     CpuArm          This;
    CpuArm              (const  This  &);
    This &  operator =  (const  This  &);
public:
    //  テストクラス。  //
    friend  class   CpuArmTest;
};

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END

#endif
