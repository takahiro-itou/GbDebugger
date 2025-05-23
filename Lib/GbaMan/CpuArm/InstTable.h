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
**      An Implementation of Arm Instruction Table.
**
**      @file       GbaMan/ArmInst.h
**/

#if !defined( GBDEBUGGER_GBAMAN_CPUARM_INCLUDED_INST_TABLE_H )
#    define   GBDEBUGGER_GBAMAN_CPUARM_INCLUDED_INST_TABLE_H

#define     arm_NI  &CpuArm::execNotImplemented
#define     arm_UI  &CpuArm::execUnknownInstruction

#define     REPEAT_16(inst)     \
    inst, inst, inst, inst, inst, inst, inst, inst,     \
    inst, inst, inst, inst, inst, inst, inst, inst

#define     REPEAT256(inst)     \
    REPEAT_16(inst), REPEAT_16(inst), REPEAT_16(inst), REPEAT_16(inst),     \
    REPEAT_16(inst), REPEAT_16(inst), REPEAT_16(inst), REPEAT_16(inst),     \
    REPEAT_16(inst), REPEAT_16(inst), REPEAT_16(inst), REPEAT_16(inst),     \
    REPEAT_16(inst), REPEAT_16(inst), REPEAT_16(inst), REPEAT_16(inst)

#define     STRLDR_INST_TABLE_IMM(P, U, B, F)                               \
    REPEAT_16((&CpuArm::execStoreLoad<0, P, U, B, F, 0, ShiftOpVoidImm>)),  \
    REPEAT_16((&CpuArm::execStoreLoad<0, P, U, B, F, 1, ShiftOpVoidImm>))

#define     STRLDR_INST_TABLE_REG(P, U, B, F)                           \
    &CpuArm::execStoreLoad<1, P, U, B, F, 0, ShiftOpLslImm>, arm_UI,    \
    &CpuArm::execStoreLoad<1, P, U, B, F, 0, ShiftOpLsrImm>, arm_UI,    \
    &CpuArm::execStoreLoad<1, P, U, B, F, 0, ShiftOpAsrImm>, arm_UI,    \
    &CpuArm::execStoreLoad<1, P, U, B, F, 0, ShiftOpRorImm>, arm_UI,    \
    &CpuArm::execStoreLoad<1, P, U, B, F, 0, ShiftOpLslImm>, arm_UI,    \
    &CpuArm::execStoreLoad<1, P, U, B, F, 0, ShiftOpLsrImm>, arm_UI,    \
    &CpuArm::execStoreLoad<1, P, U, B, F, 0, ShiftOpAsrImm>, arm_UI,    \
    &CpuArm::execStoreLoad<1, P, U, B, F, 0, ShiftOpRorImm>, arm_UI,    \
    &CpuArm::execStoreLoad<1, P, U, B, F, 1, ShiftOpLslImm>, arm_UI,    \
    &CpuArm::execStoreLoad<1, P, U, B, F, 1, ShiftOpLsrImm>, arm_UI,    \
    &CpuArm::execStoreLoad<1, P, U, B, F, 1, ShiftOpAsrImm>, arm_UI,    \
    &CpuArm::execStoreLoad<1, P, U, B, F, 1, ShiftOpRorImm>, arm_UI,    \
    &CpuArm::execStoreLoad<1, P, U, B, F, 1, ShiftOpLslImm>, arm_UI,    \
    &CpuArm::execStoreLoad<1, P, U, B, F, 1, ShiftOpLsrImm>, arm_UI,    \
    &CpuArm::execStoreLoad<1, P, U, B, F, 1, ShiftOpAsrImm>, arm_UI,    \
    &CpuArm::execStoreLoad<1, P, U, B, F, 1, ShiftOpRorImm>, arm_UI

#define     arm009  arm_NI
#define     arm00B  arm_NI
#define     arm00D  arm_NI
#define     arm00F  arm_NI
#define     arm019  arm_NI
#define     arm01B  arm_NI
#define     arm01D  arm_NI
#define     arm01F  arm_NI

#define     arm029  arm_NI
#define     arm02B  arm_NI
#define     arm02D  arm_UI
#define     arm02F  arm_UI
#define     arm039  arm_NI
#define     arm03B  arm_NI
#define     arm03D  arm_NI
#define     arm03F  arm_NI

#define     arm049  arm_UI
#define     arm04B  arm_NI
#define     arm04D  arm_UI
#define     arm04F  arm_UI
#define     arm059  arm_UI
#define     arm05B  arm_NI
#define     arm05D  arm_NI
#define     arm05F  arm_NI

#define     arm069  arm_UI
#define     arm06B  arm_NI
#define     arm06D  arm_UI
#define     arm06F  arm_UI
#define     arm079  arm_UI
#define     arm07B  arm_NI
#define     arm07D  arm_NI
#define     arm07F  arm_NI

#define     arm089  arm_NI
#define     arm08B  arm_NI
#define     arm08D  arm_UI
#define     arm08F  arm_UI
#define     arm099  arm_NI
#define     arm09B  arm_NI
#define     arm09D  arm_NI
#define     arm09F  arm_NI

#define     arm0A9  arm_NI
#define     arm0AB  arm_NI
#define     arm0AD  arm_UI
#define     arm0AF  arm_UI
#define     arm0B9  arm_NI
#define     arm0BB  arm_NI
#define     arm0BD  arm_NI
#define     arm0BF  arm_NI

#define     arm0C9  arm_NI
#define     arm0CB  arm_NI
#define     arm0CD  arm_UI
#define     arm0CF  arm_UI
#define     arm0D9  arm_NI
#define     arm0DB  arm_NI
#define     arm0DD  arm_NI
#define     arm0DF  arm_NI

#define     arm0E9  arm_NI
#define     arm0EB  arm_NI
#define     arm0ED  arm_UI
#define     arm0EF  arm_UI
#define     arm0F9  arm_NI
#define     arm0FB  arm_NI
#define     arm0FD  arm_NI
#define     arm0FF  arm_NI

#define     arm100  &CpuArm::execArm100_MrsCpsr
#define     arm120  &CpuArm::execArm120_MsrCpsrReg
#define     arm121  &CpuArm::execArm121_BX
#define     arm140  &CpuArm::execArm140_MrsSpsr
#define     arm160  &CpuArm::execArm160_MsrSpsrReg

#define     arm119  arm_UI
#define     arm11B  arm_NI
#define     arm11D  arm_NI
#define     arm11F  arm_NI

#define     arm139  arm_UI
#define     arm13B  arm_NI
#define     arm13D  arm_NI
#define     arm13F  arm_NI

#define     arm159  arm_UI
#define     arm15B  arm_NI
#define     arm15D  arm_NI
#define     arm15F  arm_NI

#define     arm179  arm_UI
#define     arm17B  arm_NI
#define     arm17D  arm_NI
#define     arm17F  arm_NI

#define     arm189  arm_UI
#define     arm18B  arm_UI
#define     arm18D  arm_UI
#define     arm18F  arm_UI
#define     arm199  arm_UI
#define     arm19B  arm_UI
#define     arm19D  arm_UI
#define     arm19F  arm_UI

#define     arm1A9  arm_UI
#define     arm1AB  arm_UI
#define     arm1AD  arm_UI
#define     arm1AF  arm_UI
#define     arm1B9  arm_UI
#define     arm1BB  arm_UI
#define     arm1BD  arm_UI
#define     arm1BF  arm_UI

#define     arm1C9  arm_UI
#define     arm1CB  arm_UI
#define     arm1CD  arm_UI
#define     arm1CF  arm_UI
#define     arm1D9  arm_UI
#define     arm1DB  arm_UI
#define     arm1DD  arm_UI
#define     arm1DF  arm_UI

#define     arm1E9  arm_UI
#define     arm1EB  arm_UI
#define     arm1ED  arm_UI
#define     arm1EF  arm_UI
#define     arm1F9  arm_UI
#define     arm1FB  arm_UI
#define     arm1FD  arm_UI
#define     arm1FF  arm_UI

#define     arm300  arm_UI
#define     arm320  &CpuArm::execArm320_MsrCpsrImm
#define     arm340  arm_UI
#define     arm360  &CpuArm::execArm360_MsrSpsrImm
#define     armALU  &CpuArm::execALUInstruction
#define     armSTR  &CpuArm::execStrLdrInstruction
#define     armLDR  &CpuArm::execStrLdrInstruction
#define     armAxx  &CpuArm::execArmAxx_B
#define     armBxx  &CpuArm::execArmBxx_BL


//  ALU 命令のうち bit7-4 が 1xx1 であるものは注意する。    //
//  bit4 が 1 なのでシフト量を示すのにレジスタを使うが、    //
//  この時 bit7 は必ず  0 である。                          //
//  そこで bit7 が  1 になっている上記のビットパターンは    //
//  あり得ないので、別の命令に割り当てられている。          //

#define     INST_TABLE_ALU_REG(CODE1, OP, S)                            \
    &CpuArm::execArithmeticLogic<0, AluOp::OP, S, ShiftOpLslImm, 0>,    \
    &CpuArm::execArithmeticLogic<0, AluOp::OP, S, ShiftOpLslReg, 1>,    \
    &CpuArm::execArithmeticLogic<0, AluOp::OP, S, ShiftOpLsrImm, 0>,    \
    &CpuArm::execArithmeticLogic<0, AluOp::OP, S, ShiftOpLsrReg, 1>,    \
    &CpuArm::execArithmeticLogic<0, AluOp::OP, S, ShiftOpAsrImm, 0>,    \
    &CpuArm::execArithmeticLogic<0, AluOp::OP, S, ShiftOpAsrReg, 1>,    \
    &CpuArm::execArithmeticLogic<0, AluOp::OP, S, ShiftOpRorImm, 0>,    \
    &CpuArm::execArithmeticLogic<0, AluOp::OP, S, ShiftOpRorReg, 1>,    \
                                                                        \
    &CpuArm::execArithmeticLogic<0, AluOp::OP, S, ShiftOpLslImm, 0>,    \
    arm##CODE1##9,                                                      \
    &CpuArm::execArithmeticLogic<0, AluOp::OP, S, ShiftOpLsrImm, 0>,    \
    arm##CODE1##B,                                                      \
    &CpuArm::execArithmeticLogic<0, AluOp::OP, S, ShiftOpAsrImm, 0>,    \
    arm##CODE1##D,                                                      \
    &CpuArm::execArithmeticLogic<0, AluOp::OP, S, ShiftOpRorImm, 0>,    \
    arm##CODE1##F

#define     INST_TABLE_ALU_TEST_IMM(CODE1, CODE2, ALU_OP)                   \
    REPEAT_16(arm##CODE1##0),                                               \
    armALU, armALU, armALU, armALU,     armALU, armALU, armALU, armALU,     \
    armALU, arm_UI, armALU, arm_UI,     armALU, arm_UI, armALU, arm_UI

#define     ALU_IMM_OP_S(OP, S)                                         \
    &CpuArm::execArithmeticLogic<1, AluOp::OP, S, ArmALUImmRor, 0>

#define     INST_TABLE_ALU_IMM(CODE1, OP, S)                            \
    REPEAT_16(ALU_IMM_OP_S(OP, S))

#define     INST_TABLE_400_7FF_STR_LDR(CODE1, CODE2)                        \
    REPEAT_16(armSTR),  REPEAT_16(armLDR)

//----------------------------------------------------------------
/**
**    命令テーブル。
**/

const   CpuArm::FnInst
CpuArm::s_armInstTable[4096] = {
    INST_TABLE_ALU_REG(00, AND, 0),         //  00.0 -- 00.F
    INST_TABLE_ALU_REG(01, AND, 1),         //  01.0 -- 01.F
    INST_TABLE_ALU_REG(02, EOR, 0),         //  02.0 -- 02.F
    INST_TABLE_ALU_REG(03, EOR, 1),         //  03.0 -- 03.F
    INST_TABLE_ALU_REG(04, SUB, 0),         //  04.0 -- 04.F
    INST_TABLE_ALU_REG(05, SUB, 1),         //  05.0 -- 05.F
    INST_TABLE_ALU_REG(06, RSB, 0),         //  06.0 -- 06.F
    INST_TABLE_ALU_REG(07, RSB, 1),         //  07.0 -- 07.F
    INST_TABLE_ALU_REG(08, ADD, 0),         //  08.0 -- 08.F
    INST_TABLE_ALU_REG(09, ADD, 1),         //  09.0 -- 09.F
    INST_TABLE_ALU_REG(0A, ADC, 0),         //  0A.0 -- 0A.F
    INST_TABLE_ALU_REG(0B, ADC, 1),         //  0B.0 -- 0B.F
    INST_TABLE_ALU_REG(0C, SBC, 0),         //  0C.0 -- 0C.F
    INST_TABLE_ALU_REG(0D, SBC, 1),         //  0D.0 -- 0D.F
    INST_TABLE_ALU_REG(0E, RSC, 0),         //  0E.0 -- 0E.F
    INST_TABLE_ALU_REG(0F, RSC, 1),         //  0F.0 -- 0F.F

    //  10.0 -- 10.F
    arm100, arm_UI, arm_UI, arm_UI,     arm_UI, arm_UI, arm_UI, arm_UI,
    arm_NI, arm_NI, arm_NI, arm_NI,     arm_NI, arm_NI, arm_NI, arm_NI,
    INST_TABLE_ALU_REG(11, TST, 1),         //  11.0 -- 11.F

    //  12.0 -- 12.F
    arm120, arm121, arm_UI, arm_UI,     arm_UI, arm_UI, arm_UI, arm_NI,
    arm_NI, arm_UI, arm_NI, arm_NI,     arm_NI, arm_NI, arm_NI, arm_NI,
    INST_TABLE_ALU_REG(13, TEQ, 1),         //  13.0 -- 13.F

    //  14.0 -- 14.F
    arm140, arm_UI, arm_UI, arm_UI,     arm_UI, arm_UI, arm_UI, arm_UI,
    arm_NI, arm_NI, arm_NI, arm_NI,     arm_NI, arm_NI, arm_NI, arm_NI,
    INST_TABLE_ALU_REG(15, CMP, 1),         //  15.0 -- 15.F

    //  16.0 -- 16.F
    arm160, arm_UI, arm_UI, arm_UI,     arm_UI, arm_UI,arm_UI, arm_UI,
    arm_NI, arm_UI, arm_NI, arm_NI,     arm_NI, arm_NI, arm_NI, arm_NI,
    INST_TABLE_ALU_REG(17, CMN, 1),         //  17.0 -- 17.F

    INST_TABLE_ALU_REG(18, ORR, 0),         //  18.0 -- 18.F
    INST_TABLE_ALU_REG(19, ORR, 1),         //  19.0 -- 19.F
    INST_TABLE_ALU_REG(1A, MOV, 0),         //  1A.0 -- 1A.F
    INST_TABLE_ALU_REG(1B, MOV, 1),         //  1B.0 -- 1B.F
    INST_TABLE_ALU_REG(1C, BIC, 0),         //  1C.0 -- 1C.F
    INST_TABLE_ALU_REG(1D, BIC, 1),         //  1D.0 -- 1D.F
    INST_TABLE_ALU_REG(1E, MVN, 0),         //  1E.0 -- 1E.F
    INST_TABLE_ALU_REG(1F, MVN, 1),         //  1F.0 -- 1F.F

    INST_TABLE_ALU_IMM(20, AND, 0),         //  20.0 -- 20.F
    INST_TABLE_ALU_IMM(21, AND, 1),         //  21.0 -- 21.F
    INST_TABLE_ALU_IMM(22, EOR, 0),         //  22.0 -- 22.F
    INST_TABLE_ALU_IMM(23, EOR, 1),         //  23.0 -- 23.F
    INST_TABLE_ALU_IMM(24, SUB, 0),         //  24.0 -- 24.F
    INST_TABLE_ALU_IMM(25, SUB, 1),         //  25.0 -- 25.F
    INST_TABLE_ALU_IMM(26, RSB, 0),         //  26.0 -- 26.F
    INST_TABLE_ALU_IMM(27, RSB, 1),         //  27.0 -- 27.F
    INST_TABLE_ALU_IMM(28, ADD, 0),         //  28.0 -- 28.F
    INST_TABLE_ALU_IMM(29, ADD, 1),         //  29.0 -- 29.F
    INST_TABLE_ALU_IMM(2A, ADC, 0),         //  2A.0 -- 2A.F
    INST_TABLE_ALU_IMM(2B, ADC, 1),         //  2B.0 -- 2B.F
    INST_TABLE_ALU_IMM(2C, SBC, 0),         //  2C.0 -- 2C.F
    INST_TABLE_ALU_IMM(2D, SBC, 1),         //  2D.0 -- 2D.F
    INST_TABLE_ALU_IMM(2E, RSC, 0),         //  2E.0 -- 2E.F
    INST_TABLE_ALU_IMM(2F, RSC, 1),         //  2F.0 -- 2F.F

    REPEAT_16(arm300),                      //  30.0 -- 30.F
    INST_TABLE_ALU_IMM(31, TST, 1),         //  31.0 -- 31.F
    REPEAT_16(arm320),                      //  32.0 -- 32.F
    INST_TABLE_ALU_IMM(33, TEQ, 1),         //  33.0 -- 33.F
    REPEAT_16(arm340),                      //  34.0 -- 34.F
    INST_TABLE_ALU_IMM(35, CMP, 1),         //  35.0 -- 35.F
    REPEAT_16(arm360),                      //  36.0 -- 36.F
    INST_TABLE_ALU_IMM(37, CMN, 1),         //  37.0 -- 37.F

    INST_TABLE_ALU_IMM(38, ORR, 0),         //  38.0 -- 38.F
    INST_TABLE_ALU_IMM(39, ORR, 1),         //  39.0 -- 39.F
    INST_TABLE_ALU_IMM(3A, MOV, 0),         //  3A.0 -- 3A.F
    INST_TABLE_ALU_IMM(3B, MOV, 1),         //  3B.0 -- 3B.F
    INST_TABLE_ALU_IMM(3C, BIC, 0),         //  3C.0 -- 3C.F
    INST_TABLE_ALU_IMM(3D, BIC, 1),         //  3D.0 -- 3D.F
    INST_TABLE_ALU_IMM(3E, MVN, 0),         //  3E.0 -- 3E.F
    INST_TABLE_ALU_IMM(3F, MVN, 1),         //  3F.0 -- 3F.F

    //  STR/LDR 命令。  //
    STRLDR_INST_TABLE_IMM(0, -1, BtWord, 0),    //  40.0 -- 41.F
    STRLDR_INST_TABLE_IMM(0, -1, BtWord, 1),    //  42.0 -- 43.F
    STRLDR_INST_TABLE_IMM(0, -1, BtByte, 0),    //  44.0 -- 45.F
    STRLDR_INST_TABLE_IMM(0, -1, BtByte, 1),    //  46.0 -- 47.F
    STRLDR_INST_TABLE_IMM(0,  1, BtWord, 0),    //  48.0 -- 49.F
    STRLDR_INST_TABLE_IMM(0,  1, BtWord, 1),    //  4A.0 -- 4B.F
    STRLDR_INST_TABLE_IMM(0,  1, BtByte, 0),    //  4C.0 -- 4D.F
    STRLDR_INST_TABLE_IMM(0,  1, BtByte, 1),    //  4E.0 -- 4F.F

    STRLDR_INST_TABLE_IMM(1, -1, BtWord, 0),    //  50.0 -- 51.F
    STRLDR_INST_TABLE_IMM(1, -1, BtWord, 1),    //  52.0 -- 53.F
    STRLDR_INST_TABLE_IMM(1, -1, BtByte, 0),    //  54.0 -- 55.F
    STRLDR_INST_TABLE_IMM(1, -1, BtByte, 1),    //  56.0 -- 57.F
    STRLDR_INST_TABLE_IMM(1,  1, BtWord, 0),    //  58.0 -- 59.F
    STRLDR_INST_TABLE_IMM(1,  1, BtWord, 1),    //  5A.0 -- 5B.F
    STRLDR_INST_TABLE_IMM(1,  1, BtByte, 0),    //  5C.0 -- 5D.F
    STRLDR_INST_TABLE_IMM(1,  1, BtByte, 1),    //  5E.0 -- 5F.F

    STRLDR_INST_TABLE_REG(0, -1, BtWord, 0),    //  60.0 -- 61.F
    STRLDR_INST_TABLE_REG(0, -1, BtWord, 1),    //  62.0 -- 63.F
    STRLDR_INST_TABLE_REG(0, -1, BtByte, 0),    //  64.0 -- 65.F
    STRLDR_INST_TABLE_REG(0, -1, BtByte, 1),    //  66.0 -- 67.F
    STRLDR_INST_TABLE_REG(0,  1, BtWord, 0),    //  68.0 -- 69.F
    STRLDR_INST_TABLE_REG(0,  1, BtWord, 1),    //  6A.0 -- 6B.F
    STRLDR_INST_TABLE_REG(0,  1, BtByte, 0),    //  6C.0 -- 6D.F
    STRLDR_INST_TABLE_REG(0,  1, BtByte, 1),    //  6E.0 -- 6F.F

    STRLDR_INST_TABLE_REG(1, -1, BtWord, 0),    //  70.0 -- 71.F
    STRLDR_INST_TABLE_REG(1, -1, BtWord, 1),    //  72.0 -- 73.F
    STRLDR_INST_TABLE_REG(1, -1, BtByte, 0),    //  74.0 -- 75.F
    STRLDR_INST_TABLE_REG(1, -1, BtByte, 1),    //  76.0 -- 77.F
    STRLDR_INST_TABLE_REG(1,  1, BtWord, 0),    //  78.0 -- 79.F
    STRLDR_INST_TABLE_REG(1,  1, BtWord, 1),    //  7A.0 -- 7B.F
    STRLDR_INST_TABLE_REG(1,  1, BtByte, 0),    //  7C.0 -- 7D.F
    STRLDR_INST_TABLE_REG(1,  1, BtByte, 1),    //  7E.0 -- 7F.F

    REPEAT256(arm_UI),      //  80.0 -- 8F.F
    REPEAT256(arm_UI),      //  90.0 -- 9F.F

    REPEAT256(armAxx),      //  A0.0 -- AF.F
    REPEAT256(armBxx),      //  B0.0 -- BF.F

    REPEAT256(arm_UI),      //  C0.0 -- CF.F
    REPEAT256(arm_UI),      //  D0.0 -- DF.F

    REPEAT256(arm_NI),      //  E0.0 -- EF.F
    REPEAT256(arm_NI),      //  F0.0 -- FF.F
};

#endif
