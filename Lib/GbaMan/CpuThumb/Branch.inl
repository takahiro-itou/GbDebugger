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
**      An Implementation of Branch Instructions.
**
**      @file       GbaMan/CpuThumb/Branch.inl
**/

#if !defined( GBDEBUGGER_GBAMAN_CPUTHUMB_INCLUDED_BRANCH_INL )
#    define   GBDEBUGGER_GBAMAN_CPUTHUMB_INCLUDED_BRANCH_INL

#if !defined( GBDEBUGGER_GBAMAN_INCLUDED_CPU_THUMB_H )
#    include    "CpuThumb.h"
#endif


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

namespace  {

}   //  End of (Unnamed) namespace.


//========================================================================
//
//    Conditional Branch  Instructions.
//

//----------------------------------------------------------------

inline
GBD_REGPARM     InstExecResult
CpuThumb::execBranchLinkHigh(
        const  OpeCode  opeCode)
{
    char    buf[512];

    const  int  ofs = static_cast<int>(opeCode & 0x07FF) << 1;
    const  GuestMemoryAddress   adr = (this->m_cpuRegs[14].dw + ofs);

    //  リターンアドレス。  //
    this->m_cpuRegs[14].dw  = (this->m_cpuRegs[15].dw - 2) | 1;
    this->m_cpuRegs[15].dw  = (this->m_nextPC = (adr & 0xFFFFFFFE)) + 2;

    sprintf(buf,
            "BL ofs=%04x, PC=%08x, LR=%08x, Next=%08x\n",
            ofs, this->m_cpuRegs[15].dw,
            this->m_cpuRegs[14].dw, this->m_nextPC
    );
    std::cerr   <<  buf;

    const   LpcReadBuf  ptr =
        this->m_manMem.getMemoryAddress(this->m_nextPC);
    prefetchAll<uint16_t>(static_cast<const uint16_t *>(ptr));

    return ( InstExecResult::SUCCESS_CONTINUE );
}

//----------------------------------------------------------------

template  <RegType SE>
GBD_REGPARM     InstExecResult
CpuThumb::execBranchLinkLow(
        const  OpeCode  opeCode)
{
    const  RegType  ofs = (opeCode & 0x07FF) << 12;
    this->m_cpuRegs[14].dw  = (this->m_cpuRegs[15].dw) + (ofs | SE);

    return ( InstExecResult::SUCCESS_CONTINUE );
}

//----------------------------------------------------------------

template  <int COND>
GBD_REGPARM     InstExecResult
CpuThumb::execConditionalBranch(
        const  OpeCode  opeCode)
{
    const  RegType  flg = (this->m_cpuRegs[16].dw >> 28) & 0x0F;
    char    buf[512];
    sprintf(buf,
            "Branch COND=%d, OpeCode=%04x, FLAGS=%d, CondCheck=%d\n",
            COND, opeCode, flg, g_condTable[COND][flg]
    );
    std::cerr   <<  buf;
    if ( g_condTable[COND][flg] ) {
        const   int8_t  val = static_cast<int8_t>(opeCode & 0xFF);
        const   GuestMemoryAddress  ofs
            = static_cast<GuestMemoryAddress>(val << 1);
        this->m_nextPC  = (this->m_cpuRegs[15].dw += ofs);
        this->m_cpuRegs[15].dw  += 2;
        sprintf(buf,
                "Branch ofs=%04x, PC=%08x\n",
                ofs, this->m_cpuRegs[15].dw
        );
        std::cerr   <<  buf;

        const   LpcReadBuf  ptr =
            this->m_manMem.getMemoryAddress(this->m_nextPC);
        prefetchAll<uint16_t>(static_cast<const uint16_t *>(ptr));
    }

    return ( InstExecResult::SUCCESS_CONTINUE );
}

//----------------------------------------------------------------

inline
GBD_REGPARM     InstExecResult
CpuThumb::execUnconditionalBranch(
        const  OpeCode  opeCode)
{
    std::cerr   <<  "Not Implemented (Branch)"  <<  std::endl;
    return ( InstExecResult::UNDEFINED_OPECODE );
}

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END

#endif
