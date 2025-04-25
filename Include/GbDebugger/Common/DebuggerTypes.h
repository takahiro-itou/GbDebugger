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
**      Type Definitions.
**
**      @file       Common/DebuggerTypes.h
**/

#if !defined( GBDEBUGGER_COMMON_INCLUDED_DEBUGGER_TYPES_H )
#    define   GBDEBUGGER_COMMON_INCLUDED_DEBUGGER_TYPES_H

#if !defined( GBDEBUGGER_COMMON_INCLUDED_DEBUGGER_PROJECT_H )
#    include    "DebuggerProject.h"
#endif

#if !defined( GBDEBUGGER_SYS_INCLUDED_CSTDINT )
#    include    <cstdint>
#    define   GBDEBUGGER_SYS_INCLUDED_CSTDINT
#endif

#if !defined( GBDEBUGGER_SYS_INCLUDED_STDDEF_H )
#    include    <stddef.h>
#    define   GBDEBUGGER_SYS_INCLUDED_STDDEF_H
#endif


GBDEBUGGER_NAMESPACE_BEGIN

//========================================================================
//
//    Type Definitions.
//

//----------------------------------------------------------------
/**
**    エラーコード。
**/

enum class  ErrCode
{
    /**   正常終了。    **/
    SUCCESS                 = 0,

    /**   異常終了。エラーの理由は不明または報告なし。  **/
    FAILURE                 = 1,

    /**   無効なインデックス指定。  **/
    INDEX_OUT_OF_RANGE      = 2,

    /**   ファイルオープンエラー。  **/
    FILE_OPEN_ERROR         = 3,

    /**   ファイル入出力エラー。    **/
    FILE_IO_ERROR           = 4,

    /**   不正なファイル形式。      **/
    FILE_INVALID_FORMAT     = 5,
};


//----------------------------------------------------------------
//
//    厳格な型の宣言を行うマクロ。
//  将来デバッグ版では厳格な型を利用できるようにするため
//  マクロを予約しておく。
//

#if defined( _DEBUG ) && !defined( GBDEBUGGER_DISABLE_STRICT_TYPES )
#    define     DECLARE_STRICT_TYPE(BaseType, DeclType)         \
    typedef     BaseType    DeclType
#else
#    define     DECLARE_STRICT_TYPE(BaseType, DeclType)         \
    typedef     BaseType    DeclType
#endif


//----------------------------------------------------------------
/**
**    読み取り専用バッファ。
**/

typedef     const  void  *      LpcReadBuf;

//--------------------------------------------------------------------------
/**
**    読み書き両用バッファ。
**/

typedef     void  *             LpWriteBuf;


//----------------------------------------------------------------
/**
**    バイト型。
**/

typedef     uint8_t             BtByte;

/**
**    バイト型の読み取り専用バッファ。
**/

typedef     const  BtByte  *    LpcByteReadBuf;

/**
**    バイト型の読み書き両用バッファ。
**/

typedef     BtByte  *           LpByteWriteBuf;


//----------------------------------------------------------------
/**
**    ゲスト上のアドレスを示す整数値。
**/

DECLARE_STRICT_TYPE(uint32_t,   GuestMemoryAddress);

//----------------------------------------------------------------
/**
**    オペコード型。
**/

DECLARE_STRICT_TYPE(uint32_t,   OpeCode);

//----------------------------------------------------------------
/**
**    レジスタの値型。
**/

DECLARE_STRICT_TYPE(uint32_t,   RegType);

//----------------------------------------------------------------
/**
**    レジスタを管理する構造体。
**/

struct  RegPair
{
    RegType     dw;
};

//========================================================================
//
//    安全なポインタ型のキャスト。
//

template  <typename  T>
T  pointer_cast(void  *  p)
{
    return ( static_cast<T>(p) );
}

template  <typename  T>
T  pointer_cast(const  void  *  p)
{
    return ( static_cast<T>(p) );
}


GBDEBUGGER_NAMESPACE_END

#endif
