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
**      An Interface of BaseCpuCore class.
**
**      @file       GbaMan/BaseCpuCore.h
**/

#if !defined( GBDEBUGGER_GBAMAN_INCLUDED_BASE_CPU_CORE_H )
#    define   GBDEBUGGER_GBAMAN_INCLUDED_BASE_CPU_CORE_H

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
//    BaseCpuCore  class.
//

class  BaseCpuCore
{

//========================================================================
//
//    Internal Type Definitions.
//

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
    BaseCpuCore(
            MemoryManager & manMem);

    //----------------------------------------------------------------
    /**   インスタンスを破棄する
    **  （デストラクタ）。
    **
    **/
    virtual  ~BaseCpuCore();

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
public:

    //----------------------------------------------------------------
    /**   現在の命令を実行する。
    **
    **/
    virtual  InstExecResult
    executeNextInst()  = 0;

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
protected:

    //----------------------------------------------------------------
    /**   命令をプリフェッチする。
    **
    **/
    template <typename T>
    inline  void
    prefetchAll(
            const  T *  ptr)
    {
        this->m_prefOpeCodes[0] = ptr[0];
        this->m_prefOpeCodes[1] = ptr[1];
    }

//========================================================================
//
//    For Internal Use Only.
//
private:

    //----------------------------------------------------------------
    /**   命令をプリフェッチする。
    **
    **/
    void
    prefetchAutoAll();

//========================================================================
//
//    Member Variables.
//
protected:

    /**   メモリマネージャ。    **/
    MemoryManager  &        m_manMem;

    /**   レジスタ。            **/
    RegPair                 m_cpuRegs[85];

    /**   次の命令のアドレス。  **/
    GuestMemoryAddress      m_nextPC;

    /**   プリフェッチ。        **/
    OpeCode                 m_prefOpeCodes[2];

    /**   現在のモード。        **/
    RegType                 m_cpuMode;

//========================================================================
//
//    Other Features.
//
private:
    typedef     BaseCpuCore     This;
    BaseCpuCore         (const  This  &);
    This &  operator =  (const  This  &);
public:
    //  テストクラス。  //
    friend  class   BaseCpuCoreTest;
};

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END

#endif
