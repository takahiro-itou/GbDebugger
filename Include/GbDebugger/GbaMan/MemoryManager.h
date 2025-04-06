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
    getMemoryAdress(
            const  uint32_t gmAddr)  const;

    //----------------------------------------------------------------
    /**   メモリの内容を読みだす。
    **
    **/
    template  <typename  T>
    inline  const  T
    readMemory(
            const  uint32_t gmAddr)  const
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
