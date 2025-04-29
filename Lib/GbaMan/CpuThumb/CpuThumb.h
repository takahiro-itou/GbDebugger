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

//========================================================================
//
//    Member Variables.
//

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
