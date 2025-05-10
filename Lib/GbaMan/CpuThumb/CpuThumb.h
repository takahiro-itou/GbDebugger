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
**      An Interface of CpuThumb class.
**
**      @file       GbaMan/CpuThumb.h
**/

#if !defined( GBDEBUGGER_GBAMAN_INCLUDED_CPU_THUMB_H )
#    define   GBDEBUGGER_GBAMAN_INCLUDED_CPU_THUMB_H

#if !defined( GBDEBUGGER_GBAMAN_INCLUDED_BASE_CPU_CORE_H )
#    include    "GbDebugger/GbaMan/BaseCpuCore.h"
#endif

#if !defined( GBDEBUGGER_COMMON_INCLUDED_DEBUGGER_TYPES_H )
#    include    "GbDebugger/Common/DebuggerTypes.h"
#endif


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

//  クラスの前方宣言。  //


//========================================================================
//
//    CpuThumb  class.
//

class  CpuThumb : public BaseCpuCore
{

//========================================================================
//
//    Internal Type Definitions.
//
private:

    typedef     BaseCpuCore     Super;

    typedef     GBD_REGPARM     InstExecResult
    (CpuThumb::* FnInst)(
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
    CpuThumb(
            GbaManager    & manGba,
            MemoryManager & manMem);

    //----------------------------------------------------------------
    /**   インスタンスを破棄する
    **  （デストラクタ）。
    **
    **/
    virtual  ~CpuThumb();

//========================================================================
//
//    Public Member Functions (Implement Pure Virtual).
//
public:

    //----------------------------------------------------------------
    /**   現在の命令を実行する。
    **
    **/
    virtual  InstExecResult
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

    template  <int OP>
    GBD_REGPARM     InstExecResult
    execBitShift(
            const  OpeCode  opeCode);

    GBD_REGPARM     InstExecResult
    execBranchLinkHigh(
            const  OpeCode  opeCode);

    template  <RegType SE>
    GBD_REGPARM     InstExecResult
    execBranchLinkLow(
            const  OpeCode  opeCode);

    template  <int COND>
    GBD_REGPARM     InstExecResult
    execConditionalBranch(
            const  OpeCode  opeCode);

    template  <int OP, int N>
    GBD_REGPARM     InstExecResult
    execOperateAddImm(
            const  OpeCode  opeCode);

    template  <int OP, int RD>
    GBD_REGPARM     InstExecResult
    execOperateImm(
            const  OpeCode  opeCode);

    template  <int Rs, int OP>
    GBD_REGPARM     InstExecResult
    execStoreLoadRelative(
            const  OpeCode  opeCode);

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
    static  const   FnInst  s_thumbInstTable[256];

//========================================================================
//
//    Other Features.
//
private:
    typedef     CpuThumb        This;
    CpuThumb            (const  This  &);
    This &  operator =  (const  This  &);
public:
    //  テストクラス。  //
    friend  class   CpuThumbTest;
};

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END

#endif
