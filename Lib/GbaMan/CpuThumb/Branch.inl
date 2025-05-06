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
        const   int8_t  ofs = static_cast<int8_t>(opeCode & 0xFF);
        this->m_cpuRegs[15].dw  += ofs;
    }
    return ( InstExecResult::UNDEFINED_OPECODE );
}

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END

#endif
