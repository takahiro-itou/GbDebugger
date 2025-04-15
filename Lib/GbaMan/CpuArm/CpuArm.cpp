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

typedef     GBD_REGPARM     int (* G_FnInst)(OpeCode opeCode);

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

    sprintf(buf, "CPSR: %08x ", this->m_cpuRegs[16].dw);
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

InstExecResult
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

    return ( InstExecResult::SUCCESS_BREAKPOINT );
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
//    命令の実行を行う関数たち。
//

template  <int BIT25, int CODE, int BIT20, int BIT4>
GBD_REGPARM     InstExecResult
CpuArm::armALUInstruction(
        const  OpeCode  opeCode)
{
    return ( InstExecResult::SUCCESS_CONTINUE );
}

GBD_REGPARM     InstExecResult
CpuArm::armA00_B(
        const  OpeCode  opeCode)
{
    //  符号拡張
    //  以下のコードと等価
    //  ofs = (opeCode & 0x00FFFFFF);
    //  if ( ofs & 0x00800000 ) ofs |= 0xFF000000;
    //  ofs <<= 2;
    int32_t ofs = (static_cast<int32_t>(opeCode & 0x00FFFFFF) << 8) >> 6;

    this->m_nextPC  = this->m_cpuRegs[15].dw  += ofs;

    //  プリフェッチを行う。    //
    prefetchAll();

    //  プリフェッチによりカウンタが１命令分進む。  //
    this->m_cpuRegs[15].dw  += 4;

    return ( InstExecResult::SUCCESS_CONTINUE );
}

GBD_REGPARM     InstExecResult
CpuArm::armUnknownInstruction(
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

//========================================================================
//
//    Member Variables.
//

/**   命令テーブル。        **/

#define     arm_UI  &CpuArm::armUnknownInstruction
#define     arm1A0  &CpuArm::armALUInstruction<0, 13, 0, 0>
#define     arm3A0  &CpuArm::armALUInstruction<1, 13, 0, 0>
#define     armA00  &CpuArm::armA00_B

#define     REPEAT_16(inst)     \
    inst, inst, inst, inst, inst, inst, inst, inst,     \
    inst, inst, inst, inst, inst, inst, inst, inst

#define     REPEAT256(inst)     \
    REPEAT_16(inst), REPEAT_16(inst), REPEAT_16(inst), REPEAT_16(inst),     \
    REPEAT_16(inst), REPEAT_16(inst), REPEAT_16(inst), REPEAT_16(inst),     \
    REPEAT_16(inst), REPEAT_16(inst), REPEAT_16(inst), REPEAT_16(inst),     \
    REPEAT_16(inst), REPEAT_16(inst), REPEAT_16(inst), REPEAT_16(inst)


const   CpuArm::FnInst
CpuArm::s_armInstTable[4096] = {
    REPEAT256(arm_UI),      //  00.0 -- 0F.F

    REPEAT_16(arm_UI),      //  10.0 -- 10.F
    REPEAT_16(arm_UI),      //  11.0 -- 11.F
    REPEAT_16(arm_UI),      //  12.0 -- 12.F
    REPEAT_16(arm_UI),      //  13.0 -- 13.F
    REPEAT_16(arm_UI),      //  14.0 -- 14.F
    REPEAT_16(arm_UI),      //  15.0 -- 15.F
    REPEAT_16(arm_UI),      //  16.0 -- 16.F
    REPEAT_16(arm_UI),      //  17.0 -- 17.F
    REPEAT_16(arm_UI),      //  18.0 -- 18.F
    REPEAT_16(arm_UI),      //  19.0 -- 19.F

    //  1A.0 -- 1A.F
    arm1A0, arm1A0, arm1A0, arm1A0,     arm1A0, arm1A0, arm1A0, arm1A0,
    arm1A0, arm1A0, arm1A0, arm1A0,     arm1A0, arm1A0, arm1A0, arm1A0,

    REPEAT_16(arm_UI),      //  1B.0 -- 1B.F
    REPEAT_16(arm_UI),      //  1C.0 -- 1C.F
    REPEAT_16(arm_UI),      //  1D.0 -- 1D.F
    REPEAT_16(arm_UI),      //  1E.0 -- 1E.F
    REPEAT_16(arm_UI),      //  1F.0 -- 1F.F

    REPEAT256(arm_UI),      //  20.0 -- 2F.F
    REPEAT_16(arm_UI),      //  30.0 -- 30.F
    REPEAT_16(arm_UI),      //  31.0 -- 31.F
    REPEAT_16(arm_UI),      //  32.0 -- 32.F
    REPEAT_16(arm_UI),      //  33.0 -- 33.F
    REPEAT_16(arm_UI),      //  34.0 -- 34.F
    REPEAT_16(arm_UI),      //  35.0 -- 35.F
    REPEAT_16(arm_UI),      //  36.0 -- 36.F
    REPEAT_16(arm_UI),      //  37.0 -- 37.F
    REPEAT_16(arm_UI),      //  38.0 -- 38.F
    REPEAT_16(arm_UI),      //  39.0 -- 39.F
    REPEAT_16(arm3A0),      //  3A.0 -- 3A.F
    REPEAT_16(arm_UI),      //  3B.0 -- 3B.F
    REPEAT_16(arm_UI),      //  3C.0 -- 3C.F
    REPEAT_16(arm_UI),      //  3D.0 -- 3D.F
    REPEAT_16(arm_UI),      //  3E.0 -- 3E.F
    REPEAT_16(arm_UI),      //  3F.0 -- 3F.F

    REPEAT256(arm_UI),      //  40.0 -- 4F.F
    REPEAT256(arm_UI),      //  50.0 -- 5F.F
    REPEAT256(arm_UI),      //  60.0 -- 6F.F
    REPEAT256(arm_UI),      //  70.0 -- 7F.F
    REPEAT256(arm_UI),      //  80.0 -- 8F.F
    REPEAT256(arm_UI),      //  90.0 -- 9F.F
    REPEAT256(armA00),      //  A0.0 -- AF.F
    REPEAT256(arm_UI),      //  B0.0 -- BF.F
    REPEAT256(arm_UI),      //  C0.0 -- CF.F
    REPEAT256(arm_UI),      //  D0.0 -- DF.F
    REPEAT256(arm_UI),      //  E0.0 -- EF.F
    REPEAT256(arm_UI),      //  F0.0 -- FF.F
};

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END
