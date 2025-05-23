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
**      @file       GbaMan/CpuArm/StoreLoad.inl
**/

#if !defined( GBDEBUGGER_GBAMAN_CPUARM_INCLUDED_STORE_LOAD_INL )
#    define   GBDEBUGGER_GBAMAN_CPUARM_INCLUDED_STORE_LOAD_INL

#if !defined( GBDEBUGGER_GBAMAN_INCLUDED_CPU_ARM_H )
#    include    "CpuArm.h"
#endif

#if !defined( GBDEBUGGER_GBAMAN_CPUARM_INCLUDED_ARM_ALU_H )
#    include    "ArmALU.h"
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

template  <int P, int U, int S, int W, int L>
GBD_REGPARM     InstExecResult
CpuArm::execOperateStack(
        const  OpeCode  opeCode)
{
    std::cerr   <<  "Not Implemented (Operate Stack)"   <<  std::endl;
    return ( InstExecResult::UNDEFINED_OPECODE );
}

//----------------------------------------------------------------

template  <int I, int P, int U, typename B,
           int BIT21, int OP, typename SHIFTOP>
GBD_REGPARM     InstExecResult
CpuArm::execStoreLoad(
        const  OpeCode  opeCode)
{
    static_assert(SHIFTOP::SHIFTW_REG == 0);

    const  int  rd  = (opeCode >> 12) & 0x0F;
    const  int  rn  = (opeCode >> 16) & 0x0F;
    int         ofs;
    RegType   & cpuFlag = mog_cpuRegs[RegIdx::CPSR].dw;

    const   bool    flag_cy = (cpuFlag & CPSR::FLAG_C) ? true : false;
    bool            fout_cy = flag_cy;

    if ( I == 0 ) {
        //  オフセットは即値。  //
        ofs = (opeCode & 0x0FFF);
    } else {
        //  オフセットはシフトされたレジスタ。  //
        ofs = getAluOp2Register<SHIFTOP>(
                opeCode, mog_cpuRegs, fout_cy);
    }

    //  U フラグ (BIT 23)   //
    ofs *= U;

    GuestMemoryAddress  gmAddr  = mog_cpuRegs[rn].dw;
    if ( P == 1 ) {
        //  PRE.    //
        gmAddr  += ofs;
    }

    char    buf[512];
    LpWriteBuf  ptr = this->m_manMem.getMemoryAddress(gmAddr);

    if ( OP == 0 ) {
        //  STR 命令。  //
#if ( GBDEBUGGER_ENABLE_TRACELOG )
        sprintf(buf, "Write to address %08x from R%d (%08x)",
                gmAddr, rd, mog_cpuRegs[rd].dw);
#endif
        *( pointer_cast<B *>(ptr) ) = static_cast<B>(mog_cpuRegs[rd].dw);
    } else {
        //  LDR 命令。  //
        mog_cpuRegs[rd].dw  = *( pointer_cast<B *>(ptr) );
#if ( GBDEBUGGER_ENABLE_TRACELOG )
        sprintf(buf, "Read from address %08x to R%d (%08x)",
                gmAddr, rd, mog_cpuRegs[rd].dw);
#endif
    }
    std::cerr   <<  buf <<  std::endl;

    if ( (P == 0) || (BIT21 == 1) ) {
        //  ライトバック。  //
        mog_cpuRegs[rn].dw  = gmAddr;
    }

    return ( InstExecResult::SUCCESS_CONTINUE );
}

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END

#endif
