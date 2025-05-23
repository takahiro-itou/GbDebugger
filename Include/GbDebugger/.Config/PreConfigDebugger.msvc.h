﻿//  -*-  coding: utf-8-with-signature;  mode: c++  -*-  //
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
**      スクリプトによる設定値が書き込まれるヘッダファイル。
**
**      @file       .Config/ConfiguredSample.h.in
**/

#if !defined( GBDEBUGGER_CONFIG_INCLUDED_CONFIGURED_DEBUGGER_H )
#    define   GBDEBUGGER_CONFIG_INCLUDED_CONFIGURED_DEBUGGER_H

//========================================================================
//
//    Name Space.
//

/**
**    スクリプトによって設定された名前空間。
**/

#define     GBDEBUGGER_CNF_NS               GbDebugger

/**
**    名前空間。
**/

#define     GBDEBUGGER_NAMESPACE            GBDEBUGGER_CNF_NS

#define     GBDEBUGGER_NAMESPACE_BEGIN      namespace  GBDEBUGGER_CNF_NS  {

#define     GBDEBUGGER_NAMESPACE_END        }

//========================================================================
//
//    Compile Features.
//

//----------------------------------------------------------------
//
//    キーワード constexpr  の検査。
//

#if ( 1 )
#    define     GBDEBUGGER_ENABLE_CONSTEXPR         1
#else
#    undef      GBDEBUGGER_ENABLE_CONSTEXPR
#endif

#if !defined( CONSTEXPR_VAR ) && !defined( CONSTEXPR_FUNC )
#    if ( GBDEBUGGER_ENABLE_CONSTEXPR )
#        define     CONSTEXPR_VAR       constexpr
#        define     CONSTEXPR_FUNC      constexpr
#    else
#        define     CONSTEXPR_VAR       const
#        define     CONSTEXPR_FUNC
#    endif
#endif

//----------------------------------------------------------------
//
//    キーワード nullptr  の検査。
//

#if ( 1 )
#    define     GBDEBUGGER_ENABLE_NULLPTR           1
#else
#    if !defined( nullptr )
#        define     nullptr     NULL
#    endif
#    undef      GBDEBUGGER_ENABLE_NULLPTR
#endif

//----------------------------------------------------------------
//
//    キーワード override の検査。
//

#if ( 1 )
#    define     GBDEBUGGER_ENABLE_OVERRIDE          1
#else
#    if !defined( override )
#        define     override
#    endif
#    undef      GBDEBUGGER_ENABLE_OVERRIDE
#endif

//----------------------------------------------------------------
//
//    キーワード static_assert  の検査。
//

#if ( 0 )
#    define     GBDEBUGGER_ENABLE_STATIC_ASSERT     1
#else
#    undef      GBDEBUGGER_ENABLE_STATIC_ASSERT
#endif

//----------------------------------------------------------------
//
//    右辺値参照の検査。
//

#if ( 0 )
#    define     GBDEBUGGER_ENABLE_RVALUEREF         1
#else
#    undef      GBDEBUGGER_ENABLE_RVALUEREF
#endif

//----------------------------------------------------------------
//
//    不要なムーブ指定に関する警告の検査。
//

#if ( 0 )
#    define     GBDEBUGGER_ERROR_PESSIMIZING_MOVE   1
#else
#    undef      GBDEBUGGER_ERROR_PESSIMIZING_MOVE
#endif

//----------------------------------------------------------------
//
//    コピーコンストラクタのデフォルト宣言の検査。
//

#if ( 0 )
#    define     GBDEBUGGER_ENABLE_COPYCTOR_DEFAULT  1
#else
#    undef      GBDEBUGGER_ENABLE_COPYCTOR_DEFAULT
#endif

//----------------------------------------------------------------
//
//    コピーコンストラクタのデリート宣言の検査。
//

#if ( 0 )
#    define     GBDEBUGGER_ENABLE_COPYCTOR_DELETE   1
#else
#    undef      GBDEBUGGER_ENABLE_COPYCTOR_DELETE
#endif

//----------------------------------------------------------------
//
//    コピー代入演算子のデフォルト宣言の検査。
//

#if ( 0 )
#    define     GBDEBUGGER_ENABLE_COPYOPEQ_DEFAULT  1
#else
#    undef      GBDEBUGGER_ENABLE_COPYOPEQ_DEFAULT
#endif

//----------------------------------------------------------------
//
//    コピー代入演算子のデリート宣言の検査。
//

#if ( 0 )
#    define     GBDEBUGGER_ENABLE_COPYOPEQ_DELETE   1
#else
#    undef      GBDEBUGGER_ENABLE_COPYOPEQ_DELETE
#endif

//----------------------------------------------------------------
//
//    ムーブコンストラクタの宣言の検査。
//

#if ( 0 )
#    define     GBDEBUGGER_ENABLE_MOVECTOR_DECLARE  1
#else
#    undef      GBDEBUGGER_ENABLE_MOVECTOR_DECLARE
#endif

//----------------------------------------------------------------
//
//    ムーブコンストラクタのデフォルト宣言の検査。
//

#if ( 0 )
#    define     GBDEBUGGER_ENABLE_MOVECTOR_DEFAULT  1
#else
#    undef      GBDEBUGGER_ENABLE_MOVECTOR_DEFAULT
#endif

//----------------------------------------------------------------
//
//    ムーブコンストラクタのデリート宣言の検査。
//

#if ( 0 )
#    define     GBDEBUGGER_ENABLE_MOVECTOR_DELETE   1
#else
#    undef      GBDEBUGGER_ENABLE_MOVECTOR_DELETE
#endif

//----------------------------------------------------------------
//
//    ムーブ代入演算子の宣言の検査。
//

#if ( 0 )
#    define     GBDEBUGGER_ENABLE_MOVEOPEQ_DECLARE  1
#else
#    undef      GBDEBUGGER_ENABLE_MOVEOPEQ_DECLARE
#endif

//----------------------------------------------------------------
//
//    ムーブ代入演算子のデフォルト宣言の検査。
//

#if ( 0 )
#    define     GBDEBUGGER_ENABLE_MOVEOPEQ_DEFAULT  1
#else
#    undef      GBDEBUGGER_ENABLE_MOVEOPEQ_DEFAULT
#endif

//----------------------------------------------------------------
//
//    ムーブ代入演算子のデリート宣言の検査。
//

#if ( 0 )
#    define     GBDEBUGGER_ENABLE_MOVEOPEQ_DELETE   1
#else
#    undef      GBDEBUGGER_ENABLE_MOVEOPEQ_DELETE
#endif

#endif
