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
    SUCCESS             = 0,

    /**   異常終了。エラーの理由は不明または報告なし。  **/
    FAILURE             = 1,

    /**   ファイルオープンエラー。  **/
    FILE_OPEN_ERROR     = 2,

    /**   ファイル入出力エラー。    **/
    FILE_IO_ERROR       = 3,

    /**   無効なインデックス指定。  **/
    INDEX_OUT_OF_RANGE  = 4
};

GBDEBUGGER_NAMESPACE_END

#endif
