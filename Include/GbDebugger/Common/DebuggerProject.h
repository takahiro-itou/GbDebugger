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
**      プロジェクトの設定。
**
**      @file       Common/DebuggerProject.h
**/

#if !defined( GBDEBUGGER_COMMON_INCLUDED_DEBUGGER_PROJECT_H )
#    define   GBDEBUGGER_COMMON_INCLUDED_DEBUGGER_PROJECT_H

//  スクリプトによる設定値が書き込まれたヘッダを読み込む。  //
#if !defined( GBDEBUGGER_CONFIG_INCLUDED_CONFIGURED_DEBUGGER_H )
#    if defined( GBDEBUGGER_USE_PRE_CONFIGURED_MSVC )
#        include    "GbDebugger/.Config/PreConfigDebugger.msvc.h"
#    else
#        include    "GbDebugger/.Config/ConfiguredDebugger.h"
#    endif
#endif

GBDEBUGGER_NAMESPACE_BEGIN

/**
**    ダミーの型宣言。
**/

typedef     int     CommonType;

GBDEBUGGER_NAMESPACE_END

#endif
