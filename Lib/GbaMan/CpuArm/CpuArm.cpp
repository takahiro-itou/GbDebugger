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


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

typedef     GBD_REGPARM     int (* FnInst)(OpeCode opeCode);

namespace  {

const char * regNames[16] = {
    "R0" , "R1" , "R2" , "R3" , "R4" , "R5", "R6", "R7",
    "R8" , "R9" , "R10", "R11", "R12", "SP", "LR", "PC"
};

/**
**    条件判定用のテーブル。
**
**    フラグは NZCV の 4bit あるから 0...15 の整数値とみなして
**  各条件についてどの数値だったら真とみなすかをテーブルにする。
**
**  例えば EQ (Z==1) であれば
**  0100, 0101, 0110, 0111, 1100, 1101, 1110, 1111
**  のいずれかであれば真になるから、
**  配列の 4, 5, 6, 7, 12, 13, 14, 15 に真を、それ以外に偽を書き込んだ
**  テーブルを用意する。
**/

CONSTEXPR_VAR   int
g_condTable[16][16] = {
    // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F
    {  0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1 },    //  EQ (F0F0)
    {  1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },    //  NE (0F0F)
    {  0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1 },    //  CS (CCCC)
    {  1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0 },    //  CC (3333)
    {  0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1 },    //  MI (FF00)
    {  1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 },    //  PL (00FF)
    {  0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1 },    //  VS (AAAA)
    {  1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0 },    //  VC (5555)
    {  0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0 },    //  HI (0C0C)
    {  1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1 },    //  LS (F3F3)
    {  1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1 },    //  GE (AA55)
    {  0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0 },    //  LT (55AA)
    {  1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0 },    //  GT (0A05)
    {  0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1 },    //  LE (F5FA)
    {  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },    //  AL (FFFF)
    {  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }     //  NV (0000)
};

int
armUnknownInstruction(OpeCode opeCode)
{
    return ( 0 );
}

#define     REPEAT_16(inst)     \
    inst, inst, inst, inst, inst, inst, inst, inst,     \
    inst, inst, inst, inst, inst, inst, inst, inst

#define     REPEAT256(inst)     \
    REPEAT_16(inst), REPEAT_16(inst), REPEAT_16(inst), REPEAT_16(inst),     \
    REPEAT_16(inst), REPEAT_16(inst), REPEAT_16(inst), REPEAT_16(inst),     \
    REPEAT_16(inst), REPEAT_16(inst), REPEAT_16(inst), REPEAT_16(inst),     \
    REPEAT_16(inst), REPEAT_16(inst), REPEAT_16(inst), REPEAT_16(inst)

FnInst  g_armInstTable[4096] = {
    REPEAT256(armUnknownInstruction),   //  00.0 - 0F.F
    REPEAT256(armUnknownInstruction),   //  10.0 - 1F.F
    REPEAT256(armUnknownInstruction),   //  20.0 - 2F.F
    REPEAT256(armUnknownInstruction),   //  30.0 - 3F.F
    REPEAT256(armUnknownInstruction),   //  40.0 - 4F.F
    REPEAT256(armUnknownInstruction),   //  50.0 - 5F.F
    REPEAT256(armUnknownInstruction),   //  60.0 - 6F.F
    REPEAT256(armUnknownInstruction),   //  70.0 - 7F.F
    REPEAT256(armUnknownInstruction),   //  80.0 - 8F.F
    REPEAT256(armUnknownInstruction),   //  90.0 - 9F.F
    REPEAT256(armUnknownInstruction),   //  A0.0 - AF.F
    REPEAT256(armUnknownInstruction),   //  B0.0 - BF.F
    REPEAT256(armUnknownInstruction),   //  C0.0 - CF.F
    REPEAT256(armUnknownInstruction),   //  D0.0 - DF.F
    REPEAT256(armUnknownInstruction),   //  E0.0 - EF.F
    REPEAT256(armUnknownInstruction),   //  F0.0 - FF.F
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
        sprintf(buf, "%4s: %08x ", regNames[i], this->m_cpuRegs[i].dw);
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
    char    buf[256];

    const  GuestMemoryAddress oldPC = this->m_nextPC;
    const  OpeCode  opeCode = this->m_prefOpeCodes[0];
    this->m_prefOpeCodes[0] = this->m_prefOpeCodes[1];

    this->m_nextPC  = this->m_cpuRegs[15].dw;
    this->m_cpuRegs[15].dw  += 4;
    prefetchNext();

    const  OpeCode  opCond  = (opeCode >> 28) & 0x0F;
    const  uint32_t    flg  = (this->m_cpuRegs[16].dw >> 28) & 0x0F;
    const  bool  condResult = g_condTable[opCond][flg];

    sprintf(buf,
            "opecode = %08x, Cond = %1x, Flag = %x, CondResult = %d\n",
            opeCode, opCond, flg, condResult);
    std::cerr   <<  buf;

    if ( condResult ) {
        //  オペコードのビット 20--27 とビット 04--07 を取り出す。  //
        //  ビット 20--27 がビット 04--11 に来るようにするので、    //
        //  ((opeCode >> 20) & 0x0FF) << 4  は、事前に計算して、    //
        //  ((opeCode >> 16) & 0xFF0) となる。                      //
        const  OpeCode  idx =
            ((opeCode >> 16) & 0xFF0) | ((opeCode >> 4) & 0x0F);
        FnInst  pfInst  = g_armInstTable[idx];
        int ret = (* pfInst)(opeCode);
        if ( ret == 0 ) {
            sprintf(buf,
                    "Undefined ARM instruction %08x at %08x\n",
                    opeCode, oldPC);
            std::cerr   <<  buf;
            return ( 0 );
        }
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
