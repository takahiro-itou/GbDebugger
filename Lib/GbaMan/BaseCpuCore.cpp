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

#include    <cstring>
#include    <iostream>
#include    <ostream>


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

namespace  {

const char * regNames[16] = {
    "R0" , "R1" , "R2" , "R3" , "R4" , "R5", "R6", "R7",
    "R8" , "R9" , "R10", "R11", "R12", "SP", "LR", "PC"
};

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
        GbaManager    & manGba,
        MemoryManager & manMem)
    : m_manGba(manGba),
      m_manMem(manMem),
      m_cpuRegs(),
      m_nextPC (),
      m_prefOpeCodes(),
      m_cpuMode(0)
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
    this->m_cpuMode = 0;

    for ( int i = 0; i < RegIdx::NUM_REGISTERS; ++ i ) {
        this->m_cpuRegs[ i].dw  = 0x00000000;
    }
    this->m_nextPC  = 0x08000000;

    prefetchAutoAll();
    this->m_cpuRegs[15].dw  = this->m_nextPC + 4;

    return ( ErrCode::SUCCESS );
}

//----------------------------------------------------------------
//    レジスタの内容をコピーする。
//

ErrCode
BaseCpuCore::getRegisters(
        RegBank  &copyBuf)  const
{
    memcpy(copyBuf, this->m_cpuRegs, sizeof(copyBuf));
    copyBuf[RegIdx::NEXT_PC].dw = this->m_nextPC;
    return ( ErrCode::SUCCESS );
}

//----------------------------------------------------------------
//    レジスタの内容をダンプする。
//

std::ostream  &
BaseCpuCore::printRegisters(
        std::ostream  & outStr)  const
{
    char    buf[256];

    for ( int i = 0; i < 16; ++ i ) {
        sprintf(buf, "%4s: %08x ", regNames[i], this->m_cpuRegs[i].dw);
        outStr  <<  buf;
        if ( (i & 3) == 3 ) {
            outStr  <<  std::endl;
        }
    }

    sprintf(buf, "CPSR: %08x ", this->m_cpuRegs[16].dw);
    outStr  <<  buf;
    sprintf(buf, "Next: %08x\n", this->m_nextPC);
    outStr  <<  buf;

    return ( outStr );
}

//----------------------------------------------------------------
//    レジスタの内容を設定する。
//

ErrCode
BaseCpuCore::setRegisters(
        const  RegBank  &cpuRegs)
{
    memcpy(this->m_cpuRegs, cpuRegs, sizeof(cpuRegs));

    this->m_nextPC  = cpuRegs[RegIdx::NEXT_PC].dw;
    prefetchAutoAll();

    return ( ErrCode::SUCCESS );
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
BaseCpuCore::prefetchAutoAll()
{
    const   LpcReadBuf  ptr = this->m_manMem.getMemoryAddress(this->m_nextPC);
    if ( LIKELY(this->m_cpuMode) ) {
        prefetchAll<uint16_t>(static_cast<const uint16_t *>(ptr));
    } else {
        prefetchAll<OpeCode>(static_cast<const OpeCode *>(ptr));
    }
}

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END
