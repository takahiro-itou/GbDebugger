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
    const  RegType  flg  = (this->m_cpuRegs[16].dw >> 28) & 0x0F;
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

/**   命令テーブル。        **/

#define     arm_UI  &CpuArm::execUnknownInstruction
#define     arm100  &CpuArm::execArm100_MsrCpsr
#define     arm120  &CpuArm::execArm120_MrsCpsrReg
#define     arm140  &CpuArm::execArm140_MsrSpsr
#define     arm160  &CpuArm::execArm160_MrsSpsrReg
#define     arm300  arm_UI
#define     arm320  &CpuArm::execArm320_MrsCpsrImm
#define     arm340  arm_UI
#define     arm360  &CpuArm::execArm360_MrsSpsrImm
#define     armALU  &CpuArm::execALUInstruction
#define     armSTR  &CpuArm::execLdrStrInstruction
#define     armLDR  &CpuArm::execLdrStrInstruction
#define     armAxx  &CpuArm::execArmAxx_B

#define     REPEAT_16(inst)     \
    inst, inst, inst, inst, inst, inst, inst, inst,     \
    inst, inst, inst, inst, inst, inst, inst, inst

#define     REPEAT256(inst)     \
    REPEAT_16(inst), REPEAT_16(inst), REPEAT_16(inst), REPEAT_16(inst),     \
    REPEAT_16(inst), REPEAT_16(inst), REPEAT_16(inst), REPEAT_16(inst),     \
    REPEAT_16(inst), REPEAT_16(inst), REPEAT_16(inst), REPEAT_16(inst),     \
    REPEAT_16(inst), REPEAT_16(inst), REPEAT_16(inst), REPEAT_16(inst)


#define     INST_TABLE_000_1FF(CODE1, CODE2, ALU_OP)                        \
    armALU, armALU, armALU, armALU,     armALU, armALU, armALU, armALU,     \
    armALU, arm_UI, armALU, arm_UI,     armALU, arm_UI, armALU, arm_UI,     \
    armALU, armALU, armALU, armALU,     armALU, armALU, armALU, armALU,     \
    armALU, arm_UI, armALU, arm_UI,     armALU, arm_UI, armALU, arm_UI

#define     INST_TABLE_ALU_TEST(CODE1, CODE2, ALU_OP)                       \
    REPEAT_16(arm##CODE1##0),                                               \
    armALU, armALU, armALU, armALU,     armALU, armALU, armALU, armALU,     \
    armALU, arm_UI, armALU, arm_UI,     armALU, arm_UI, armALU, arm_UI

#define     INST_TABLE_200_3FF(CODE1, CODE2, ALU_OP)                        \
    REPEAT_16(armALU),  REPEAT_16(armALU)

#define     INST_TABLE_400_7FF(CODE1, CODE2)          \
    REPEAT_16(armSTR),  REPEAT_16(armLDR)

const   CpuArm::FnInst
CpuArm::s_armInstTable[4096] = {
    INST_TABLE_000_1FF(00, 01, AND),        //  00.0 -- 01.F
    INST_TABLE_000_1FF(02, 03, EOR),        //  02.0 -- 03.F
    INST_TABLE_000_1FF(04, 05, SUB),        //  04.0 -- 05.F
    INST_TABLE_000_1FF(06, 07, RSB),        //  06.0 -- 07.F
    INST_TABLE_000_1FF(08, 09, ADD),        //  08.0 -- 09.F
    INST_TABLE_000_1FF(0A, 0B, ADC),        //  0A.0 -- 0B.F
    INST_TABLE_000_1FF(0C, 0D, SBC),        //  0C.0 -- 0D.F
    INST_TABLE_000_1FF(0E, 0F, RSC),        //  0E.0 -- 0F.F

    //  10.0 -- 11.F
    INST_TABLE_ALU_TEST(10, 11, TST),       //  10.0 -- 11.F
    INST_TABLE_ALU_TEST(12, 13, TEQ),       //  12.0 -- 13.F
    INST_TABLE_ALU_TEST(14, 15, CMP),       //  14.0 -- 15.F
    INST_TABLE_ALU_TEST(16, 17, CMN),       //  16.0 -- 17.F
    INST_TABLE_000_1FF(18, 19, ORR),        //  18.0 -- 19.F
    INST_TABLE_000_1FF(1A, 1B, MOV),        //  1A.0 -- 1B.F
    INST_TABLE_000_1FF(1C, 1D, BIC),        //  1C.0 -- 1D.F
    INST_TABLE_000_1FF(1E, 1F, MVN),        //  1E.0 -- 1F.F

    INST_TABLE_200_3FF(20, 21, AND),        //  20.0 -- 21.F
    INST_TABLE_200_3FF(22, 23, EOR),        //  22.0 -- 23.F
    INST_TABLE_200_3FF(24, 25, SUB),        //  24.0 -- 25.F
    INST_TABLE_200_3FF(26, 27, RSB),        //  26.0 -- 27.F
    INST_TABLE_200_3FF(28, 29, ADD),        //  28.0 -- 29.F
    INST_TABLE_200_3FF(2A, 2B, ADC),        //  2A.0 -- 2B.F
    INST_TABLE_200_3FF(2C, 2D, SBC),        //  2C.0 -- 2D.F
    INST_TABLE_200_3FF(2E, 2F, RSC),        //  2E.0 -- 2F.F

    INST_TABLE_ALU_TEST(30, 31, TST),       //  30.0 -- 31.F
    INST_TABLE_ALU_TEST(32, 33, TEQ),       //  32.0 -- 33.F
    INST_TABLE_ALU_TEST(34, 35, CMP),       //  34.0 -- 35.F
    INST_TABLE_ALU_TEST(36, 37, CMN),       //  36.0 -- 37.F
    INST_TABLE_200_3FF(38, 39, ORR),        //  38.0 -- 39.F
    INST_TABLE_200_3FF(3A, 3B, MOV),        //  3A.0 -- 3B.F
    INST_TABLE_200_3FF(3C, 3D, BIC),        //  3C.0 -- 3D.F
    INST_TABLE_200_3FF(3E, 3F, MVN),        //  3E.0 -- 3F.F

    //  STR/LDR 命令。  //
    INST_TABLE_400_7FF(40, 41),             //  40.0 -- 41.F
    INST_TABLE_400_7FF(42, 43),             //  42.0 -- 43.F
    INST_TABLE_400_7FF(44, 45),             //  44.0 -- 45.F
    INST_TABLE_400_7FF(46, 47),             //  46.0 -- 47.F
    INST_TABLE_400_7FF(48, 49),             //  48.0 -- 49.F
    INST_TABLE_400_7FF(4A, 4B),             //  4A.0 -- 4B.F
    INST_TABLE_400_7FF(4C, 4D),             //  4C.0 -- 4D.F
    INST_TABLE_400_7FF(4E, 4F),             //  4E.0 -- 4F.F

    INST_TABLE_400_7FF(50, 51),             //  50.0 -- 51.F
    INST_TABLE_400_7FF(52, 53),             //  52.0 -- 53.F
    INST_TABLE_400_7FF(54, 55),             //  54.0 -- 55.F
    INST_TABLE_400_7FF(56, 57),             //  56.0 -- 57.F
    INST_TABLE_400_7FF(58, 59),             //  58.0 -- 59.F
    INST_TABLE_400_7FF(5A, 5B),             //  5A.0 -- 5B.F
    INST_TABLE_400_7FF(5C, 5D),             //  5C.0 -- 5D.F
    INST_TABLE_400_7FF(5E, 5F),             //  5E.0 -- 5F.F

    REPEAT256(arm_UI),      //  60.0 -- 6F.F
    REPEAT256(arm_UI),      //  70.0 -- 7F.F
    REPEAT256(arm_UI),      //  80.0 -- 8F.F
    REPEAT256(arm_UI),      //  90.0 -- 9F.F
    REPEAT256(armAxx),      //  A0.0 -- AF.F
    REPEAT256(arm_UI),      //  B0.0 -- BF.F
    REPEAT256(arm_UI),      //  C0.0 -- CF.F
    REPEAT256(arm_UI),      //  D0.0 -- DF.F
    REPEAT256(arm_UI),      //  E0.0 -- EF.F
    REPEAT256(arm_UI),      //  F0.0 -- FF.F
};

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END
