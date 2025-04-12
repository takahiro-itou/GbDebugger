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
**      An Implementation of CpuArm class.
**
**      @file       GbaMan/CpuArm.cpp
**/

#include    "CpuArm.h"

#include    "GbDebugger/GbaMan/GbaManager.h"
#include    "GbDebugger/GbaMan/MemoryManager.h"

#include    <iostream>
#include    <ostream>


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

namespace  {

const char * regs[16] = {
    "R0" , "R1" , "R2" , "R3" , "R4" , "R5", "R6", "R7",
    "R8" , "R9" , "R10", "R11", "R12", "SP", "LR", "PC"
};

}   //  End of (Unnamed) namespace.


//========================================================================
//
//    CpuArm  class.
//

//========================================================================
//
//    Constructor(s) and Destructor.
//

//----------------------------------------------------------------
//    インスタンスを初期化する
//  （デフォルトコンストラクタ）。
//

CpuArm::CpuArm(
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

CpuArm::~CpuArm()
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
CpuArm::doHardReset()
{
    for ( int i = 0; i < 48; ++ i ) {
        this->m_cpuRegs[ i].dw  = 0x00000000;
    }
    this->m_nextPC  = 0x08000000;

    prefetchAll();
    this->m_cpuRegs[15].dw  = this->m_nextPC + 4;

    return ( ErrCode::SUCCESS );
}

//----------------------------------------------------------------
//    レジスタの内容をダンプする。
//

std::ostream  &
CpuArm::printRegisters(
        std::ostream  & outStr)  const
{
    char    buf[256];

    for ( int i = 0; i < 16; ++ i ) {
        sprintf(buf, "%4s: %08x ", regs[i], this->m_cpuRegs[i].dw);
        outStr  <<  buf;
        if ( (i & 3) == 3 ) {
            outStr  <<  std::endl;
        }
    }

    sprintf(buf, "CPSR: %08x ", this->m_cpuRegs[16]);
    outStr  <<  buf;
    sprintf(buf, "Next: %08x\n", this->m_nextPC);
    outStr  <<  buf;

    return ( outStr );
}

//========================================================================
//
//    Public Member Functions.
//

//----------------------------------------------------------------
//    現在の命令を実行する。
//

int
CpuArm::executeNextInst()
{
    const  OpeCode  opeCode = this->m_prefOpeCodes[0];
    this->m_prefOpeCodes[0] = this->m_prefOpeCodes[1];

    this->m_nextPC  = this->m_cpuRegs[15].dw;
    this->m_cpuRegs[15].dw  += 4;
    prefetchNext();

    const  OpeCode  opCond  = (opeCode >> 28);
    bool        condResult  = true;

    //  フラグを展開する。
    uint32_t    cpsr    = this->m_cpuRegs[16].dw;
    bool        cpuFlgN = (cpsr & 0x80000000) ? true : false;
    bool        cpuFlgZ = (cpsr & 0x40000000) ? true : false;
    bool        cpuFlgC = (cpsr & 0x20000000) ? true : false;
    bool        cpuFlgV = (cpsr & 0x10000000) ? true : false;

    if ( UNLIKELY(opCond != 0x0E) ) {
        switch ( opCond ) {
        case  0x00:     //  EQ
            condResult  = cpuFlgZ;
            break;
        case  0x01:     //  NE
            condResult  = !cpuFlgZ;
            break;
        case  0x02:     //  CS
            condResult  = cpuFlgC;
            break;
        case  0x03:     //  CC
            condResult  = !cpuFlgC;
            break;
        case  0x04:     //  MI
            condResult  = cpuFlgN;
            break;
        case  0x05:     //  PL
            condResult  = !cpuFlgN;
            break;
        case  0x06:     //  VS
            condResult  = cpuFlgV;
            break;
        case  0x07:     //  VC
            condResult  = !cpuFlgV;
            break;
        case  0x08:     //  HI
            condResult  = cpuFlgC && !cpuFlgZ;
            break;
        case  0x09:     //  LS
            condResult  = !cpuFlgC || cpuFlgZ;
            break;
        case  0x0A:     //  GE
            condResult  = (cpuFlgN == cpuFlgV);
            break;
        case  0x0B:     //  LT
            condResult  = (cpuFlgN != cpuFlgV);
            break;
        case  0x0C:     //  GT
            condResult  = !cpuFlgZ && (cpuFlgN == cpuFlgV);
            break;
        case  0x0D:     //  LE
            condResult  = cpuFlgZ || (cpuFlgN != cpuFlgV);
            break;
        case  0x0E:     //  AL
            condResult  = true;
            break;
        case  0x0F:     //  NV (Never)
        default:
            condResult  = false;
            break;
        }
    }

    std::cerr   <<  "opecode = "    <<  opeCode
                <<  ", cond = "     <<  opCond
                <<  ", condResult = "   <<  condResult
                <<  std::endl;

    if ( condResult ) {
    }

    return ( 0 );
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

//----------------------------------------------------------------
//    命令を実行する。
//

int
CpuArm::executeInst(
        const  OpeCode  opeCode)
{
    return ( 0 );
}

//----------------------------------------------------------------
//    命令をプリフェッチする。
//

inline  void
CpuArm::prefetchAll()
{
    this->m_prefOpeCodes[0] =
            this->m_manMem.readMemory<OpeCode>(this->m_nextPC);
    this->m_prefOpeCodes[1] =
            this->m_manMem.readMemory<OpeCode>(this->m_nextPC + 4);
}

//----------------------------------------------------------------
//    次の命令をプリフェッチする。
//

inline  void
CpuArm::prefetchNext()
{
    this->m_prefOpeCodes[1] =
            this->m_manMem.readMemory<OpeCode>(this->m_nextPC + 4);
}

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END
