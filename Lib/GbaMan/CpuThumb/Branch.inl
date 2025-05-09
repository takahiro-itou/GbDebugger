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

inline
GBD_REGPARM     InstExecResult
CpuThumb::execBranchLinkHigh(
        const  OpeCode  opeCode)
{
    const  int  ofs = (opeCode & 0x07FF) << 1;
    const  uint32_t adr = (this->m_cpuRegs[14].dw + (ofs << 1));

    //  リターンアドレス。  //
    this->m_cpuRegs[14].dw  = (this->m_cpuRegs[15].dw - 2) | 1;
    this->m_nextPC  = (adr & ~1);
    this->m_cpuRegs[15].dw  = (this->m_nextPC + 2);

    return ( InstExecResult::SUCCESS_CONTINUE );
}

template  <RegType SE>
GBD_REGPARM     InstExecResult
CpuThumb::execBranchLinkLow(
        const  OpeCode  opeCode)
{
    const  RegType  ofs = (opeCode & 0x07FF);
    this->m_cpuRegs[14].dw  = (this->m_cpuRegs[15].dw) + ((ofs << 12) | SE);

    return ( InstExecResult::SUCCESS_CONTINUE );
}

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

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END

#endif
