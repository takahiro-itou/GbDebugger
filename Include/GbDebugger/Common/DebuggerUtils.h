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
**      Utility Macros/Functions.
**
**      @file       Common/DebuggerUtils.h
**/

#if !defined( GBDEBUGGER_COMMON_INCLUDED_DEBUGGER_UTILS_H )
#    define   GBDEBUGGER_COMMON_INCLUDED_DEBUGGER_UTILS_H

#if !defined( GBDEBUGGER_COMMON_INCLUDED_DEBUGGER_TYPES_H )
#    include    "DebuggerTypes.h"
#endif


//================================================================
//
//    補助マクロ
//

//
//    分岐予測に関するマクロ
//

#if defined( __GNUC__ )
#    define     LIKELY(x)       __builtin_expect(!!(x), 1)
#    define     UNLIKELY(x)     __builtin_expect(!!(x), 0)
#else
#    define     LIKELY(x)       (x)
#    define     UNLIKELY(x)     (x)
#endif


/**
**    引数を可能な限りレジスタで渡すようにする。
**/

#if defined(__i386__) || defined(__x86_64__)
#    define     GBD_REGPARM     __attribute__((regparm(2)))
#else
#    define     GBD_REGPARM
#endif

GBDEBUGGER_NAMESPACE_BEGIN

GBDEBUGGER_NAMESPACE_END

#endif
