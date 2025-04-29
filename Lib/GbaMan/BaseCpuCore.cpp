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
**      An Implementation of BaseCpuCore class.
**
**      @file       GbaMan/BaseCpuCore.cpp
**/

#include    "GbDebugger/GbaMan/BaseCpuCore.h"

#include    "GbDebugger/GbaMan/MemoryManager.h"

#include    "GbDebugger/Common/DebuggerUtils.h"

#include    <iostream>
#include    <ostream>


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

namespace  {

}   //  End of (Unnamed) namespace.


//========================================================================
//
//    BaseCpuCore  class.
//

//========================================================================
//
//    Constructor(s) and Destructor.
//

//----------------------------------------------------------------
//    インスタンスを初期化する
//  （コンストラクタ）。
//

BaseCpuCore::BaseCpuCore(
        MemoryManager & manMem)
    : m_manMem(manMem),
      m_cpuRegs(),
      m_nextPC (),
      m_prefOpeCodes()
{
}

//----------------------------------------------------------------
//    インスタンスを破棄する
//  （デストラクタ）。
//

BaseCpuCore::~BaseCpuCore()
{
}

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

//----------------------------------------------------------------
//    レジスタをリセットする。
//

ErrCode
BaseCpuCore::doHardReset()
{
}

//----------------------------------------------------------------
//    現在の命令を実行する。
//

InstExecResult
BaseCpuCore::executeNextInst()
{
}

//----------------------------------------------------------------
//    レジスタの内容をダンプする。
//

std::ostream  &
BaseCpuCore::printRegisters(
        std::ostream  & outStr)  const
{
}

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

//----------------------------------------------------------------
//    命令をプリフェッチする。
//

void
BaseCpuCore::prefetchAll()
{
}

//----------------------------------------------------------------
//    次の命令をプリフェッチする。
//

void
BaseCpuCore::prefetchNext()
{
}

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END
