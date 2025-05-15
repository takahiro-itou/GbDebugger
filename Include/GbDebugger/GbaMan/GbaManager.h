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
**      An Interface of GbaManager class.
**
**      @file       GbaMan/GbaManager.h
**/

#if !defined( GBDEBUGGER_GBAMAN_INCLUDED_GBA_MANAGER_H )
#    define   GBDEBUGGER_GBAMAN_INCLUDED_GBA_MANAGER_H

#if !defined( GBDEBUGGER_COMMON_INCLUDED_DEBUGGER_TYPES_H )
#    include    "GbDebugger/Common/DebuggerTypes.h"
#endif

#if !defined( GBDEBUGGER_GBAMAN_INCLUDED_CPU_UTILS_H )
#    include    "CpuUtils.h"
#endif

#if !defined( GBDEBUGGER_GBAMAN_INCLUDED_MEMORY_MANAGER_H )
#    include    "MemoryManager.h"
#endif

#if !defined( GBDEBUGGER_SYS_STL_INCLUDED_IOSFWD )
#    include    <iosfwd>
#    define   GBDEBUGGER_SYS_STL_INCLUDED_IOSFWD
#endif


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

//  クラスの前方宣言。  //
class   BaseCpuCore;
class   BaseDisCpu;
class   CpuArm;
class   CpuThumb;
class   DisArm;
class   DisThumb;


//========================================================================
//
//    GbaManager  class.
//

class  GbaManager
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
    **  （デフォルトコンストラクタ）。
    **
    **/
    GbaManager();

    //----------------------------------------------------------------
    /**   インスタンスを破棄する
    **  （デストラクタ）。
    **
    **/
    virtual  ~GbaManager();

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
    /**   現在動作しているインスタンスを閉じる。
    **
    **  @return     エラーコードを返す。
    **      -   異常終了の場合は、
    **          エラーの種類を示す非ゼロ値を返す。
    **      -   正常終了の場合は、ゼロを返す。
    **/
    virtual  ErrCode
    closeInstance();

    //----------------------------------------------------------------
    /**   リセットを行う。
    **
    **  @return     エラーコードを返す。
    **      -   異常終了の場合は、
    **          エラーの種類を示す非ゼロ値を返す。
    **      -   正常終了の場合は、ゼロを返す。
    **/
    virtual  ErrCode
    doHardReset();

    //----------------------------------------------------------------
    /**   現在の命令を実行する。
    **
    **/
    virtual  InstExecResult
    executeCurrentInst();

    //----------------------------------------------------------------
    /**   現在のクロック数を取得する。
    **
    **/
    virtual  uint64_t
    getCpuTotalTicks()  const;

    //----------------------------------------------------------------
    /**   プログラムカウンタを取得する。
    **
    **/
    virtual  GuestMemoryAddress
    getNextPC()  const;

    //----------------------------------------------------------------
    /**   ROM ファイルを読み込む。
    **
    **  @param [in] szFileName    ファイル名。
    **  @return     エラーコードを返す。
    **      -   異常終了の場合は、
    **          エラーの種類を示す非ゼロ値を返す。
    **      -   正常終了の場合は、ゼロを返す。
    **/
    virtual  ErrCode
    openRomFile(
            const   char *  szFileName);

    //----------------------------------------------------------------
    /**   レジスタの内容をダンプする。
    **
    **/
    virtual  std::ostream  &
    printRegisters(
            std::ostream  & outStr)  const;

    //----------------------------------------------------------------
    /**   ニーモニックを表示する。
    **
    **/
    virtual  std::ostream  &
    disassembleArm(
            std::ostream       &outStr,
            GuestMemoryAddress  gmAddr)  const;

    //----------------------------------------------------------------
    /**   ニーモニックを表示する。
    **
    **/
    virtual  std::ostream  &
    writeMnemonicCurrent(
            std::ostream       &outStr,
            GuestMemoryAddress  gmAddr)  const;

    //----------------------------------------------------------------
    /**   ニーモニックを表示する。
    **
    **/
    virtual  std::ostream  &
    disassembleThumb(
            std::ostream       &outStr,
            GuestMemoryAddress  gmAddr)  const;

//========================================================================
//
//    Public Member Functions.
//
public:

    //----------------------------------------------------------------
    /**   CPU モードを切り替える。
    **
    **/
    ErrCode
    changeCpuMode(
            const  RegType  thumbState);

    //----------------------------------------------------------------
    /**   メモリの内容を読みだす。
    **
    **/
    template  <typename  T>
    inline  const  T
    readMemory(
            const   GuestMemoryAddress  gmAddr)  const
    {
        return  this->m_manMem.readMemory<T>(gmAddr);
    }

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
private:

    /**   メモリ空間。  **/
    MemoryManager   m_manMem;

    /**   プロセッサ。  **/
    BaseCpuCore  *  m_cpuCur;

    CpuArm  *       m_cpuMod0;

    CpuThumb  *     m_cpuMod1;

    BaseDisCpu  *   m_disCur;

    /**   CPU モード。  **/
    RegType         m_cpuMode;

//========================================================================
//
//    Other Features.
//
private:
    typedef     GbaManager      This;
    GbaManager          (const  This  &);
    This &  operator =  (const  This  &);
public:
    //  テストクラス。  //
    friend  class   GbaManagerTest;
};

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END

#endif
