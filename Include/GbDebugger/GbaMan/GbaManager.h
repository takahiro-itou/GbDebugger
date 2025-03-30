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

#if !defined( GBDEBUGGER_GBAMAN_INCLUDED_MEMORY_TABLE_H )
#    include    "MemoryTable.h"
#endif

#if !defined( GBDEBUGGER_SYS_STL_INCLUDED_IOSFWD )
#    include    <iosfwd>
#    define   GBDEBUGGER_SYS_STL_INCLUDED_IOSFWD
#endif


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

//  クラスの前方宣言。  //


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
public:

    struct  RegPair
    {
        uint32_t    dw;
    };

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
    /**   ニーモニックを表示する。
    **
    **/
    virtual  std::ostream  &
    disassembleArm(
            std::ostream  & outStr,
            const uint32_t  addr);

    //----------------------------------------------------------------
    /**   ニーモニックを表示する。
    **
    **/
    virtual  std::ostream  &
    disassembleThumb(
            std::ostream  & outStr,
            const uint32_t  addr);

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
private:

    /**   BIOS.     **/
    uint8_t *       m_memBios;

    uint8_t *       m_memWorkRam;

    uint8_t *       m_memInternalRam;

    uint8_t *       m_memPaletteRam;

    uint8_t *       m_memIO;

    uint8_t *       m_memVRam;

    uint8_t *       m_memOam;

    uint8_t *       m_memRom;

    uint8_t *       m_memSave;

    /**   メモリ空間。  **/
    MemoryTable     m_tblMem[256];

    /**   レジスタ。    **/
    RegPair         m_regs[16];

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
