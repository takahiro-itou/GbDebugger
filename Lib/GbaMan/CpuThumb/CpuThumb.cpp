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

#include    "GbDebugger/GbaMan/MemoryManager.h"

#include    <iostream>


//========================================================================
//
//    Implementation of Inlines.
//

#include    "Branch.inl"
#include    "OperateRegister.inl"
#include    "StoreLoad.inl"

//========================================================================


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

namespace  {

}   //  End of (Unnamed) namespace.


//========================================================================
//
//    CpuThumb  class.
//

#include    "InstTable.h"

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

    {
        const  GuestMemoryAddress oldPC = this->m_nextPC;
        const  OpeCode  opeCode = this->m_prefOpeCodes[0];
        this->m_prefOpeCodes[0] = this->m_prefOpeCodes[1];

        mog_cpuInfo.prefetchActive  = false;
        mog_cpuInfo.clockCounts     = 0;

        this->m_nextPC  = mog_cpuRegs[RegIdx::PC].dw;
        mog_cpuRegs[RegIdx::PC].dw  += 2;
        prefetchNext();

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
        if ( mog_cpuInfo.clockCounts == 0 ) {
            mog_cpuInfo.clockCounts = 1;
        }
        mog_cpuInfo.totalClocks += mog_cpuInfo.clockCounts;
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

//----------------------------------------------------------------
//    次の命令をプリフェッチする。
//

void
CpuThumb::prefetchNext()
{
    this->m_prefOpeCodes[1] =
            this->m_manMem.readMemory<uint16_t>(this->m_nextPC + 2);
}

//----------------------------------------------------------------
//    プログラムカウンタを変更する命令の処理。
//

GBD_REGPARM     InstExecResult
CpuThumb::modifyProgramCounter(
        const  RegType  valNew)
{
    if ( !(valNew & 1) ) {
        //  ARM モードに切り替え。  //
        this->m_nextPC  = valNew & ~3;
        mog_cpuRegs[RegIdx::CPSR].dw    &= ~CPSR::FLAG_T;
        mog_cpuRegs[RegIdx::PC].dw      = (this->m_nextPC + 4);
        return ( InstExecResult::UNDEFINED_OPECODE );
    }

    //  THUMB モード。  //
    this->m_nextPC  = valNew & ~1;
    mog_cpuRegs[RegIdx::PC].dw  = (this->m_nextPC + 2);

    const   LpcReadBuf  ptr =
        this->m_manMem.getMemoryAddress(this->m_nextPC);
    prefetchAll<uint16_t>(static_cast<const uint16_t *>(ptr));

    return ( InstExecResult::SUCCESS_CONTINUE );
}


}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END
