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
**      An Implementation of CpuThumb class.
**
**      @file       GbaMan/CpuThumb.cpp
**/

#include    "CpuThumb.h"

#include    <iostream>


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

namespace  {

}   //  End of (Unnamed) namespace.


//========================================================================
//
//    CpuThumb  class.
//

#include    "ThumbInstTable.h"

//========================================================================
//
//    Constructor(s) and Destructor.
//

//----------------------------------------------------------------
//    インスタンスを初期化する
//  （デフォルトコンストラクタ）。
//

CpuThumb::CpuThumb(
        GbaManager    & manGba,
        MemoryManager & manMem)
    : Super(manGba, manMem)
{
    this->m_cpuMode = CPSR::FLAG_T;
}

//----------------------------------------------------------------
//    インスタンスを破棄する
//  （デストラクタ）。
//

CpuThumb::~CpuThumb()
{
}

//========================================================================
//
//    Public Member Functions (Implement Pure Virtual).
//

//----------------------------------------------------------------
//    現在の命令を実行する。
//

InstExecResult
CpuThumb::executeNextInst()
{
    char    buf[256];

    const  GuestMemoryAddress oldPC = this->m_nextPC;
    const  OpeCode  opeCode = this->m_prefOpeCodes[0];
    this->m_prefOpeCodes[0] = this->m_prefOpeCodes[1];

    this->m_nextPC  = this->m_cpuRegs[RegIdx::PC].dw;
    this->m_cpuRegs[RegIdx::PC].dw  += 2;
    //  prefetchNext();

    const  OpeCode  idx = (opeCode >> 8) & 0x00FF;
    FnInst  pfInst  = s_thumbInstTable[idx];
    //  InstExecResult  ret = (this ->* pfInst)(opeCode);
    InstExecResult  ret = InstExecResult::UNDEFINED_OPECODE;
    if ( pfInst != nullptr ) {
        ret = (this ->* pfInst)(opeCode);
    }
    if ( ret == InstExecResult::UNDEFINED_OPECODE ) {
        sprintf(buf,
                "Undefined Thumb instruction %04x (%03x) at %08x\n",
                opeCode, idx, oldPC);
        std::cerr   <<  buf;
        return ( InstExecResult::UNDEFINED_OPECODE );
    }

    return ( InstExecResult::SUCCESS_BREAKPOINT );
}

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

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END
