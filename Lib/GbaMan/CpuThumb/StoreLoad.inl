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
**      @file       GbaMan/CpuThumb/StoreLoad.inl
**/

#if !defined( GBDEBUGGER_GBAMAN_CPUTHUMB_INCLUDED_STORE_LOAD_IML )
#    define   GBDEBUGGER_GBAMAN_CPUTHUMB_INCLUDED_STORE_LOAD_IML

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

template  <int Rs>
GBD_REGPARM     InstExecResult
CpuThumb::execAddressingRelative(
        const  OpeCode  opeCode)
{
    const  int      rd  = ((opeCode >> 8) & 0x07);
    const  RegType  nn  = ((opeCode     ) & 0xFF);

    if ( Rs == 15 ) {
        mog_cpuRegs[rd].dw  = (mog_cpuRegs[Rs].dw & ~2) + nn;
    } else {
        mog_cpuRegs[rd].dw  = mog_cpuRegs[Rs].dw + nn;
    }

    return ( InstExecResult::SUCCESS_CONTINUE );
}

//----------------------------------------------------------------

template  <int RB>
GBD_REGPARM     InstExecResult
CpuThumb::execMultipleLoad(
        const  OpeCode  opeCode)
{
#if ( GBDEBUGGER_ENABLE_TRACELOG )
    char    buf[512];
#endif

    int cnt = 0;
    GuestMemoryAddress  gmAddr  = mog_cpuRegs[RB].dw;
    RegType  *  ptr = static_cast<RegType *>(
            this->m_manMem.getMemoryAddress(gmAddr));

    for ( int bit = 0; bit < 8; ++ bit ) {
        if ( (opeCode >> bit) & 1 ) {
            mog_cpuRegs[bit].dw = * (ptr ++);

#if ( GBDEBUGGER_ENABLE_TRACELOG )
            sprintf(buf, "Read from address %08x to R%d (%08x)",
                    gmAddr, bit, mog_cpuRegs[bit].dw);
            std::cerr   <<  buf <<  std::endl;
#endif
            ++ cnt;
            gmAddr  += 4;
            if ( bit == RB ) {
                std::cerr   <<  "Destructed Rb:"    <<  RB  <<  std::endl;
                return ( InstExecResult::UNDEFINED_OPECODE );
            }
        }
    }
    mog_cpuRegs[RB].dw  = gmAddr;

    return ( InstExecResult::SUCCESS_CONTINUE );
}

//----------------------------------------------------------------

template  <int RB>
GBD_REGPARM     InstExecResult
CpuThumb::execMultipleStore(
        const  OpeCode  opeCode)
{
#if ( GBDEBUGGER_ENABLE_TRACELOG )
    char    buf[512];
#endif

    int cnt = 0;
    GuestMemoryAddress  gmAddr  = mog_cpuRegs[RB].dw;
    RegType  *  ptr = static_cast<RegType *>(
            this->m_manMem.getMemoryAddress(gmAddr));

    for ( int bit = 0; bit < 8; ++ bit ) {
        if ( (opeCode >> bit) & 1 ) {
#if ( GBDEBUGGER_ENABLE_TRACELOG )
            sprintf(buf, "Write to address %08x from R%d (%08x)",
                    gmAddr, bit, mog_cpuRegs[bit].dw);
            std::cerr   <<  buf <<  std::endl;
#endif
            * (ptr ++)  = mog_cpuRegs[bit].dw;
            ++ cnt;
            gmAddr  += 4;
        }
    }
    mog_cpuRegs[RB].dw  = gmAddr;;

    return ( InstExecResult::SUCCESS_CONTINUE );
}

//----------------------------------------------------------------

template  <int OP, int PCLR>
GBD_REGPARM     InstExecResult
CpuThumb::execPushPop(
        const  OpeCode  opeCode)
{
#if ( GBDEBUGGER_ENABLE_TRACELOG )
    char    buf[512];
#endif

    int cnt = 0;
    GuestMemoryAddress  gmAddr  = (mog_cpuRegs[RegIdx::R13].dw & ~3);
    RegType  *  ptr = static_cast<RegType *>(
            this->m_manMem.getMemoryAddress(gmAddr));

    if ( OP == 0 ) {
        if ( PCLR >= 0 ) {
            gmAddr  -= 4;

#if ( GBDEBUGGER_ENABLE_TRACELOG )
            sprintf(buf, "Write to address %08x from R%d (%08x)",
                    gmAddr, PCLR, mog_cpuRegs[PCLR].dw);
            std::cerr   <<  buf <<  std::endl;
#endif
            * (-- ptr)  = mog_cpuRegs[PCLR].dw;
            ++ cnt;
        }
        for ( int bit = 7; bit >= 0; -- bit ) {
            if ( (opeCode >> bit) & 1 ) {
                gmAddr  -= 4;

#if ( GBDEBUGGER_ENABLE_TRACELOG )
                sprintf(buf, "Write to address %08x from R%d (%08x)",
                        gmAddr, bit, mog_cpuRegs[bit].dw);
                std::cerr   <<  buf <<  std::endl;
#endif
                * (-- ptr)  = mog_cpuRegs[bit].dw;
                ++ cnt;
            }
        }
    } else {
        for ( int bit = 0; bit <= 7; ++ bit ) {
            if ( (opeCode >> bit) & 1 ) {
                mog_cpuRegs[bit].dw = *(ptr ++);

#if ( GBDEBUGGER_ENABLE_TRACELOG )
                sprintf(buf, "Read from address %08x to R%d (%08x)",
                        gmAddr, bit, mog_cpuRegs[bit].dw);
                std::cerr   <<  buf <<  std::endl;
#endif
                ++ cnt;
                gmAddr  += 4;
            }
        }
        if ( PCLR >= 0 ) {
            mog_cpuRegs[PCLR].dw    = *(ptr ++);

#if ( GBDEBUGGER_ENABLE_TRACELOG )
            sprintf(buf, "Read from address %08x to R%d (%08x)",
                    gmAddr, PCLR, mog_cpuRegs[PCLR].dw);
            std::cerr   <<  buf <<  std::endl;
#endif
            ++ cnt;
            gmAddr  += 4;
        }
    }
    mog_cpuRegs[RegIdx::R13].dw = gmAddr;

    return ( InstExecResult::SUCCESS_CONTINUE );
}

//----------------------------------------------------------------

template  <int Rs, int OP, RegType M>
GBD_REGPARM     InstExecResult
CpuThumb::execStoreLoadRelative(
        const  OpeCode  opeCode)
{
    const  int      rd  = (opeCode >> 8) & 0x07;
    const  OpeCode  nn  = (opeCode & 0x00FF);

#if ( GBDEBUGGER_ENABLE_TRACELOG )
    char    buf[512];
#endif

    GuestMemoryAddress  gmAddr  = (mog_cpuRegs[Rs].dw & M) + (nn << 2);
    LpWriteBuf  ptr = this->m_manMem.getMemoryAddress(gmAddr);

    if ( OP == 0 ) {
        //  STR 命令。  //
#if ( GBDEBUGGER_ENABLE_TRACELOG )
        sprintf(buf, "Write to address %08x from R%d (%08x)",
                gmAddr, rd, mog_cpuRegs[rd].dw);
        std::cerr   <<  buf <<  std::endl;
#endif
        *( pointer_cast<RegType *>(ptr) ) = (mog_cpuRegs[rd].dw);
    } else {
        //  LDR 命令。  //
        mog_cpuRegs[rd].dw  = *( pointer_cast<const RegType *>(ptr) );
#if ( GBDEBUGGER_ENABLE_TRACELOG )
        sprintf(buf, "Read from address %08x to R%d (%08x)",
                gmAddr, rd, mog_cpuRegs[rd].dw);
        std::cerr   <<  buf <<  std::endl;
#endif
    }

    return ( InstExecResult::SUCCESS_CONTINUE );
}

//----------------------------------------------------------------

template  <int OP, typename B>
GBD_REGPARM     InstExecResult
CpuThumb::execStoreLoadWithImmOffset(
        const  OpeCode  opeCode)
{
    const  int      rd  = ((opeCode     ) & 0x07);
    const  int      rb  = ((opeCode >> 3) & 0x07);
    const  RegType  nn  = ((opeCode >> 6) & 0x1F);

#if ( GBDEBUGGER_ENABLE_TRACELOG )
    char    buf[512];
#endif

    GuestMemoryAddress  gmAddr  = mog_cpuRegs[rb].dw + (sizeof(B) * nn);
    LpWriteBuf  ptr = this->m_manMem.getMemoryAddress(gmAddr);

    switch ( OP ) {
    case  0:        //  STR
#if ( GBDEBUGGER_ENABLE_TRACELOG )
        sprintf(buf, "Write to address %08x from R%d (%08x)",
                gmAddr, rd, mog_cpuRegs[rd].dw);
        std::cerr   <<  buf <<  std::endl;
#endif
        *( pointer_cast<B *>(ptr) ) = static_cast<B>(mog_cpuRegs[rd].dw);

        break;
    case  1:        //  LDR
        mog_cpuRegs[rd].dw  = *( pointer_cast<B *>(ptr) );
#if ( GBDEBUGGER_ENABLE_TRACELOG )
        sprintf(buf, "Read from address %08x to R%d (%08x)",
                gmAddr, rd, mog_cpuRegs[rd].dw);
        std::cerr   <<  buf <<  std::endl;
#endif
        break;
    }

    return ( InstExecResult::SUCCESS_CONTINUE );
}

//----------------------------------------------------------------

template  <int OP, typename B>
GBD_REGPARM     InstExecResult
CpuThumb::execStoreLoadWithRegOffset(
        const  OpeCode  opeCode)
{
    const  int  rd  = ((opeCode     ) & 0x07);
    const  int  rb  = ((opeCode >> 3) & 0x07);
    const  int  ro  = ((opeCode >> 6) & 0x07);

#if ( GBDEBUGGER_ENABLE_TRACELOG )
    char    buf[512];
#endif

    GuestMemoryAddress  gmAddr  = mog_cpuRegs[rb].dw + mog_cpuRegs[ro].dw;
    LpWriteBuf  ptr = this->m_manMem.getMemoryAddress(gmAddr);

    switch ( OP & 1 ) {
    case  0:        //  STR
#if ( GBDEBUGGER_ENABLE_TRACELOG )
        sprintf(buf, "Write to address %08x from R%d (%08x)",
                gmAddr, rd, mog_cpuRegs[rd].dw);
        std::cerr   <<  buf <<  std::endl;
#endif
        *( pointer_cast<B *>(ptr) ) = static_cast<B>(mog_cpuRegs[rd].dw);
        break;
    case  1:        //  LDR
        mog_cpuRegs[rd].dw  = *( pointer_cast<B *>(ptr) );
#if ( GBDEBUGGER_ENABLE_TRACELOG )
        sprintf(buf, "Read from address %08x to R%d (%08x)",
                gmAddr, rd, mog_cpuRegs[rd].dw);
        std::cerr   <<  buf <<  std::endl;
#endif
        break;
    }

    return ( InstExecResult::SUCCESS_CONTINUE );
}

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END

#endif
