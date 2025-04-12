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
**      An Interface of MemoryManager class.
**
**      @file       GbaMan/MemoryManager.h
**/

#if !defined( GBDEBUGGER_GBAMAN_INCLUDED_MEMORY_MANAGER_H )
#    define   GBDEBUGGER_GBAMAN_INCLUDED_MEMORY_MANAGER_H

#if !defined( GBDEBUGGER_COMMON_INCLUDED_DEBUGGER_TYPES_H )
#    include    "GbDebugger/Common/DebuggerTypes.h"
#endif

#if !defined( GBDEBUGGER_GBAMAN_INCLUDED_MEMORY_TABLE_H )
#    include    "MemoryTable.h"
#endif


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

//  クラスの前方宣言。  //


//========================================================================
//
//    MemoryManager  class.
//

class  MemoryManager
{

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
    MemoryManager();

    //----------------------------------------------------------------
    /**   インスタンスを破棄する
    **  （デストラクタ）。
    **
    **/
    virtual  ~MemoryManager();

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
    /**   メモリアドレスを計算する。
    **
    **    ゲストのアドレスに対応するホストのアドレスを計算する。
    **
    **  @param [in] gmAddr    ゲストのメモリアドレス。
    **/
    void  *
    getMemoryAddress(
            const   GuestMemoryAddress  gmAddr)  const;

    //----------------------------------------------------------------
    /**   メモリの内容を読みだす。
    **
    **/
    template  <typename  T>
    inline  const  T
    readMemory(
            const   GuestMemoryAddress  gmAddr)  const
    {
        const T  *  ptr = static_cast<const T *>(getMemoryAddress(gmAddr));
        return ( *ptr );
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

    /**   0x00000000-0x00003FFF :  16 KiB : BIOS.    **/
    LpByteWriteBuf      m_memBios;

    /**   0x02000000-0x0203FFFF : 256 KiB : EWRAM.  **/
    LpByteWriteBuf      m_memWRam;

    /**   0x03000000-0x03007FFF :  32 KiB : IWRAM.  **/
    LpByteWriteBuf      m_memIRam;

    /**   0x04000000-0x040003FE :   1 KiB : I/O.    **/
    LpByteWriteBuf      m_memIO;

    /**   0x05000000-0x050003FF :   1 KiB : パレット。  **/
    LpByteWriteBuf      m_memPRam;

    /**   0x06000000-0x06017FFF :  96 KiB : VRAM.   **/
    LpByteWriteBuf      m_memVRam;

    /**   0x07000000-0x070003FF :   1 KiB : OAM.    **/
    LpByteWriteBuf      m_memOam;

    /**   0x08000000-0x09FFFFFF :  32 MiB : ROM.    **/
    LpByteWriteBuf      m_memRom;

    /**   0x0E000000-0x0E00FFFF :  64 KiB : SRAM.   **/
    LpByteWriteBuf      m_memSave;

    /**   メモリ空間のマップ。  **/
    MemoryTable         m_tblMem[256];

//========================================================================
//
//    Other Features.
//
private:
    typedef     MemoryManager       This;
    MemoryManager       (const  This  &);
    This &  operator =  (const  This  &);
public:
    //  テストクラス。  //
    friend  class   MemoryManagerTest;
};

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END

#endif
