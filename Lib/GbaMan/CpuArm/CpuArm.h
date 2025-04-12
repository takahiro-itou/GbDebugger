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
**      An Interface of CpuArm class.
**
**      @file       GbaMan/CpuArm.h
**/

#if !defined( GBDEBUGGER_GBAMAN_INCLUDED_CPU_ARM_H )
#    define   GBDEBUGGER_GBAMAN_INCLUDED_CPU_ARM_H

#if !defined( GBDEBUGGER_COMMON_INCLUDED_DEBUGGER_TYPES_H )
#    include    "GbDebugger/Common/DebuggerTypes.h"
#endif


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

//  クラスの前方宣言。  //
class   MemoryManager;


//========================================================================
//
//    CpuArm  class.
//

class  CpuArm
{

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
public:

    //----------------------------------------------------------------
    /**   現在の命令を実行する。
    **
    **/
    int
    executeNextInst();

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
    inline  void
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

    /**   メモリマネージャ。    **/
    MemoryManager  &        m_manMem;

    /**   レジスタ。            **/
    RegPair                 m_cpuRegs[16];

    /**   次の命令のアドレス。  **/
    GuestMemoryAddress      m_nextPC;

    /**   プリフェッチ。        **/
    OpeCode                 m_prefOpeCodes[2];

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
