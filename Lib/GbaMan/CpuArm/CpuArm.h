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

#if !defined( GBDEBUGGER_COMMON_INCLUDED_DEBUGGER_UTILS_H )
#    include    "GbDebugger/Common/DebuggerUtils.h"
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
//    Internal Type Definitions.
//

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
public:

    //----------------------------------------------------------------
    /**   レジスタをリセットする。
    **
    **  @return     エラーコードを返す。
    **      -   異常終了の場合は、
    **          エラーの種類を示す非ゼロ値を返す。
    **      -   正常終了の場合は、ゼロを返す。
    **/
    virtual  ErrCode
    doHardReset();

    //----------------------------------------------------------------
    /**   レジスタの内容をダンプする。
    **
    **/
    virtual  std::ostream  &
    printRegisters(
            std::ostream  & outStr)  const;

//========================================================================
//
//    Public Member Functions.
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

    GBD_REGPARM     InstExecResult
    execALUInstruction(
            const  OpeCode  opeCode);

    GBD_REGPARM     InstExecResult
    execArm100_MsrCpsr(
            const  OpeCode  opeCode);

    GBD_REGPARM     InstExecResult
    execArm120_MrsCpsrReg(
            const  OpeCode  opeCode);

    GBD_REGPARM     InstExecResult
    execArm121_BX(
            const  OpeCode  opeCode);

    GBD_REGPARM     InstExecResult
    execArm140_MsrSpsr(
            const  OpeCode  opeCode);

    GBD_REGPARM     InstExecResult
    execArm160_MrsSpsrReg(
            const  OpeCode  opeCode);

    GBD_REGPARM     InstExecResult
    execArm320_MrsCpsrImm(
            const  OpeCode  opeCode);

    GBD_REGPARM     InstExecResult
    execArm360_MrsSpsrImm(
            const  OpeCode  opeCode);

    GBD_REGPARM     InstExecResult
    execArmA00_B(
            const  OpeCode  opeCode);

    GBD_REGPARM     InstExecResult
    execLdrStrInstruction(
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
    RegPair                 m_cpuRegs[85];

    /**   次の命令のアドレス。  **/
    GuestMemoryAddress      m_nextPC;

    /**   プリフェッチ。        **/
    OpeCode                 m_prefOpeCodes[2];

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
