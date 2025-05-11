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
**      An Implementation of Store/Load Instructions.
**
**      @file       GbaMan/ThumbStrLdr.inl
**/

#if !defined( GBDEBUGGER_GBAMAN_INCLUDED_THUMB_STRLDR_INLH )
#    define   GBDEBUGGER_GBAMAN_INCLUDED_THUMB_STRLDR_INLH

#if !defined( GBDEBUGGER_GBAMAN_INCLUDED_CPU_THUMB_H )
#    include    "CpuThumb.h"
#endif


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

namespace  {

}   //  End of (Unnamed) namespace.


//========================================================================
//
//    Store/Load  Instructions.
//

//----------------------------------------------------------------

template  <int RB>
GBD_REGPARM     InstExecResult
CpuThumb::execMultipleLoad(
        const  OpeCode  opeCode)
{
    char    buf[512];

    int cnt = 0;
    GuestMemoryAddress  gmAddr  = this->m_cpuRegs[RB].dw;
    RegType  *  ptr = static_cast<RegType *>(
            this->m_manMem.getMemoryAddress(gmAddr));

    for ( int bit = 0; bit < 8; ++ bit ) {
        if ( (opeCode >> bit) & 1 ) {
            this->m_cpuRegs[bit].dw = * (ptr ++);

            sprintf(buf, "Read from address %08x to R%d (%08x)",
                    gmAddr, bit, this->m_cpuRegs[bit].dw);
            std::cerr   <<  buf <<  std::endl;

            ++ cnt;
            gmAddr  += 4;
            if ( bit == RB ) {
                std::cerr   <<  "Destructed Rb:"    <<  RB  <<  std::endl;
                return ( InstExecResult::UNDEFINED_OPECODE );
            }
        }
    }
    this->m_cpuRegs[RB].dw  = gmAddr;

    return ( InstExecResult::SUCCESS_CONTINUE );
}

//----------------------------------------------------------------

template  <int RB>
GBD_REGPARM     InstExecResult
CpuThumb::execMultipleStore(
        const  OpeCode  opeCode)
{
    char    buf[512];

    int cnt = 0;
    GuestMemoryAddress  gmAddr  = this->m_cpuRegs[RB].dw;
    RegType  *  ptr = static_cast<RegType *>(
            this->m_manMem.getMemoryAddress(gmAddr));

    for ( int bit = 0; bit < 8; ++ bit ) {
        if ( (opeCode >> bit) & 1 ) {
            sprintf(buf, "Write to address %08x from R%d (%08x)",
                    gmAddr, bit, this->m_cpuRegs[bit].dw);
            std::cerr   <<  buf <<  std::endl;

            * (ptr ++)  = this->m_cpuRegs[bit].dw;
            ++ cnt;
            gmAddr  += 4;
        }
    }
    this->m_cpuRegs[RB].dw  = gmAddr;;

    return ( InstExecResult::SUCCESS_CONTINUE );
}

//----------------------------------------------------------------

template  <int Rs, int OP>
GBD_REGPARM     InstExecResult
CpuThumb::execStoreLoadRelative(
        const  OpeCode  opeCode)
{
    const  int      rd  = (opeCode >> 8) & 0x07;
    const  OpeCode  nn  = (opeCode & 0x00FF);

    GuestMemoryAddress  gmAddr  = this->m_cpuRegs[Rs].dw + (nn << 2);
    char    buf[512];
    LpWriteBuf  ptr = this->m_manMem.getMemoryAddress(gmAddr);

    if ( OP == 0 ) {
        //  STR 命令。  //
        sprintf(buf, "Write to address %08x from R%d (%08x)",
                gmAddr, rd, this->m_cpuRegs[rd].dw);
        *( pointer_cast<RegType *>(ptr) ) = (this->m_cpuRegs[rd].dw);
    } else {
        //  LDR 命令。  //
        this->m_cpuRegs[rd].dw  = *( pointer_cast<const RegType *>(ptr) );
        sprintf(buf, "Read from address %08x to R%d (%08x)",
                gmAddr, rd, this->m_cpuRegs[rd].dw);
    }
    std::cerr   <<  buf <<  std::endl;

    return ( InstExecResult::SUCCESS_CONTINUE );
}

//----------------------------------------------------------------

template  <int OP, typename B>
GBD_REGPARM     InstExecResult
CpuThumb::execStoreLoadWithImmOffset(
        const  OpeCode  opeCode)
{
    std::cerr   <<  "Not Implemented (StoreLoad With ImmOffs)"  <<  std::endl;
    return ( InstExecResult::UNDEFINED_OPECODE );
}

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END

#endif
