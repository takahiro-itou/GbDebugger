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
**      @file       GbaMan/ArmStrLdr.cpp
**/

#include    "CpuArm.h"
#include    "ArmALU.h"

#include    "GbDebugger/GbaMan/MemoryManager.h"

#include    <iostream>


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

namespace  {

typedef     GBD_REGPARM     InstExecResult
(* FnStrLdrInst)(
        const  OpeCode  opeCode,
        RegPair         cpuRegs[],
        MemoryManager & manMem,
        RegType       & cpuFlag);

template  <int I, int P, int U, typename B, int BIT21,
           int OP, typename SHIFTOP>
GBD_REGPARM     InstExecResult
armStrLdrInstruction(
        const  OpeCode  opeCode,
        RegPair         cpuRegs[],
        MemoryManager & manMem,
        RegType       & cpuFlag)
{
    static_assert(SHIFTOP::SHIFTW_REG == 0);

    const  int  rd  = (opeCode >> 12) & 0x0F;
    const  int  rn  = (opeCode >> 16) & 0x0F;
    int         ofs;

    const   bool    flag_cy = (cpuFlag & CPSR::FLAG_C) ? true : false;
    bool            fout_cy = flag_cy;

    if ( I == 0 ) {
        //  オフセットは即値。  //
        ofs = (opeCode & 0x0FFF);
    } else {
        //  オフセットはシフトされたレジスタ。  //
        ofs = getAluOp2Register<SHIFTOP>(
                opeCode, cpuRegs, fout_cy);
    }

    //  U フラグ (BIT 23)   //
    ofs *= U;

    GuestMemoryAddress  gmAddr  = cpuRegs[rn].dw;
    if ( P == 1 ) {
        //  PRE.    //
        gmAddr  += ofs;
    }

    char    buf[512];
    LpWriteBuf  ptr = manMem.getMemoryAddress(gmAddr);

    if ( OP == 0 ) {
        //  STR 命令。  //
        sprintf(buf, "Write to address %08x from R%d (%08x)",
                gmAddr, rd, cpuRegs[rd].dw);
        *( pointer_cast<B *>(ptr) ) = static_cast<B>(cpuRegs[rd].dw);
    } else {
        //  LDR 命令。  //
        cpuRegs[rd].dw  = *( pointer_cast<B *>(ptr) );
        sprintf(buf, "Read from address %08x to R%d (%08x)",
                gmAddr, rd, cpuRegs[rd].dw);
    }
    std::cerr   <<  buf <<  std::endl;

    if ( (P == 0) || (BIT21 == 1) ) {
        //  ライトバック。  //
        cpuRegs[rn].dw  = gmAddr;
    }

    return ( InstExecResult::SUCCESS_CONTINUE );
}

#define     ARMSTRLDR_INST_TABLE(I, P, U, B)                \
    armStrLdrInstruction<I, P, U, B, 0, 0, ShiftOpLslImm>,  \
    armStrLdrInstruction<I, P, U, B, 0, 0, ShiftOpLsrImm>,  \
    armStrLdrInstruction<I, P, U, B, 0, 0, ShiftOpAsrImm>,  \
    armStrLdrInstruction<I, P, U, B, 0, 0, ShiftOpRorImm>,  \
    armStrLdrInstruction<I, P, U, B, 0, 1, ShiftOpLslImm>,  \
    armStrLdrInstruction<I, P, U, B, 0, 1, ShiftOpLsrImm>,  \
    armStrLdrInstruction<I, P, U, B, 0, 1, ShiftOpAsrImm>,  \
    armStrLdrInstruction<I, P, U, B, 0, 1, ShiftOpRorImm>,  \
    armStrLdrInstruction<I, P, U, B, 1, 0, ShiftOpLslImm>,  \
    armStrLdrInstruction<I, P, U, B, 1, 0, ShiftOpLsrImm>,  \
    armStrLdrInstruction<I, P, U, B, 1, 0, ShiftOpAsrImm>,  \
    armStrLdrInstruction<I, P, U, B, 1, 0, ShiftOpRorImm>,  \
    armStrLdrInstruction<I, P, U, B, 1, 1, ShiftOpLslImm>,  \
    armStrLdrInstruction<I, P, U, B, 1, 1, ShiftOpLsrImm>,  \
    armStrLdrInstruction<I, P, U, B, 1, 1, ShiftOpAsrImm>,  \
    armStrLdrInstruction<I, P, U, B, 1, 1, ShiftOpRorImm>

CONSTEXPR_VAR   FnStrLdrInst
g_armStrLdrInstTable[256] = {
    ARMSTRLDR_INST_TABLE(0, 0, -1, BtWord),
    ARMSTRLDR_INST_TABLE(0, 0, -1, BtByte),
    ARMSTRLDR_INST_TABLE(0, 0,  1, BtWord),
    ARMSTRLDR_INST_TABLE(0, 0,  1, BtByte),
    ARMSTRLDR_INST_TABLE(0, 1, -1, BtWord),
    ARMSTRLDR_INST_TABLE(0, 1, -1, BtByte),
    ARMSTRLDR_INST_TABLE(0, 1,  1, BtWord),
    ARMSTRLDR_INST_TABLE(0, 1,  1, BtByte),

    ARMSTRLDR_INST_TABLE(1, 0, -1, BtWord),
    ARMSTRLDR_INST_TABLE(1, 0, -1, BtByte),
    ARMSTRLDR_INST_TABLE(1, 0,  1, BtWord),
    ARMSTRLDR_INST_TABLE(1, 0,  1, BtByte),
    ARMSTRLDR_INST_TABLE(1, 1, -1, BtWord),
    ARMSTRLDR_INST_TABLE(1, 1, -1, BtByte),
    ARMSTRLDR_INST_TABLE(1, 1,  1, BtWord),
    ARMSTRLDR_INST_TABLE(1, 1,  1, BtByte),
};

}   //  End of (Unnamed) namespace.


//========================================================================
//
//    ArmStrLdr  Instructions.
//

GBD_REGPARM     InstExecResult
CpuArm::execStrLdrInstruction(
        const  OpeCode  opeCode)
{
    //  オペコードから下記のビットを取り出す。          //
    //  bit     25  オフセットがレジスタか即値か。      //
    //  bit     24  Post/Pre  フラグ。                  //
    //  bit     23  オフセットがプラスかマイナスか。    //
    //  bit     22  アクセスがバイト単位か。            //
    //  bit     21                                      //
    //  bit     20  ストアかロードか。                  //
    //  bit  6-- 5  シフトの種類 (LSL, LSR, ASR, ROR)   //
    const  OpeCode  idx =
        ((opeCode >> 18) & 0x00FC) | ((opeCode >> 5) & 0x03);
    FnStrLdrInst    pfInst  = g_armStrLdrInstTable[idx];

    char    buf[512];
    sprintf(buf,
            "opeCode = %08x, idx = %03x, pfInst = %p\n",
            opeCode, idx, pfInst);
    std::cerr   <<  buf;

    if ( pfInst == nullptr ) {
        return ( InstExecResult::UNDEFINED_OPECODE );
    }

    return  (* pfInst)(
            opeCode, this->m_cpuRegs, this->m_manMem, this->m_cpuRegs[16].dw);
}

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END
