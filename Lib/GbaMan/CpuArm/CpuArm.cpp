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

#include    "GbDebugger/GbaMan/MemoryManager.h"

#include    "GbDebugger/Common/DebuggerUtils.h"

#include    <iostream>
#include    <ostream>


//========================================================================
//
//    Implementation of Inlines.
//

#include    "StoreLoad.inl"

//========================================================================


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

typedef     GBD_REGPARM     int (* G_FnInst)(OpeCode opeCode);

namespace  {

}   //  End of (Unnamed) namespace.


//========================================================================
//
//    CpuArm  class.
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

CpuArm::CpuArm(
        GbaManager    & manGba,
        MemoryManager & manMem)
    : Super(manGba, manMem)
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

//----------------------------------------------------------------
//    現在の命令を実行する。
//

InstExecResult
CpuArm::executeNextInst()
{
    char    buf[256];

    {
        const  GuestMemoryAddress oldPC = this->m_nextPC;
        const  OpeCode  opeCode = this->m_prefOpeCodes[0];
        this->m_prefOpeCodes[0] = this->m_prefOpeCodes[1];

        this->m_nextPC  = mog_cpuRegs[RegIdx::PC].dw;
        mog_cpuRegs[RegIdx::PC].dw  += 4;
        prefetchNext();

        const  OpeCode  opCond  = (opeCode >> 28) & 0x0F;
        const  RegType  flg  = (mog_cpuRegs[RegIdx::CPSR].dw >> 28) & 0x0F;
        const  bool  condResult = g_condTable[opCond][flg];

#if ( GBDEBUGGER_ENABLE_TRACELOG )
        sprintf(buf,
                "opecode = %08x, Cond = %1x, Flag = %x, CondResult = %d\n",
                opeCode, opCond, flg, condResult);
        std::cerr   <<  buf;
#endif

        if ( condResult ) {
            //  オペコードのビット 20--27 とビット 04--07 を取り出す。  //
            //  ビット 20--27 がビット 04--11 に来るようにするので、    //
            //  ((opeCode >> 20) & 0x0FF) << 4  は、事前に計算して、    //
            //  ((opeCode >> 16) & 0xFF0) となる。                      //
            const  OpeCode  idx =
                ((opeCode >> 16) & 0xFF0) | ((opeCode >> 4) & 0x0F);
            FnInst  pfInst  = s_armInstTable[idx];
            InstExecResult  ret = (this ->* pfInst)(opeCode);
            if ( ret == InstExecResult::UNDEFINED_OPECODE ) {
                sprintf(buf,
                        "Undefined ARM instruction %08x (%03x) at %08x\n",
                        opeCode, idx, oldPC);
                std::cerr   <<  buf;
                return ( InstExecResult::UNDEFINED_OPECODE );
            }
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
//    命令の実行を行う関数たち。
//

GBD_REGPARM     InstExecResult
CpuArm::execUnknownInstruction(
        const  OpeCode  opeCode)
{
    return ( InstExecResult::UNDEFINED_OPECODE );
}

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

void
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

//========================================================================
//
//    Member Variables.
//

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END
