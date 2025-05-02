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

#if !defined( GBDEBUGGER_GBAMAN_INCLUDED_ARM_INST_H )
#    define   GBDEBUGGER_GBAMAN_INCLUDED_ARM_INST_H

#define     arm_UI  &CpuArm::execUnknownInstruction

#define     arm009  arm_UI
#define     arm00B  arm_UI
#define     arm00D  arm_UI
#define     arm00F  arm_UI
#define     arm019  arm_UI
#define     arm01B  arm_UI
#define     arm01D  arm_UI
#define     arm01F  arm_UI

#define     arm029  arm_UI
#define     arm02B  arm_UI
#define     arm02D  arm_UI
#define     arm02F  arm_UI
#define     arm039  arm_UI
#define     arm03B  arm_UI
#define     arm03D  arm_UI
#define     arm03F  arm_UI

#define     arm049  arm_UI
#define     arm04B  arm_UI
#define     arm04D  arm_UI
#define     arm04F  arm_UI
#define     arm059  arm_UI
#define     arm05B  arm_UI
#define     arm05D  arm_UI
#define     arm05F  arm_UI

#define     arm069  arm_UI
#define     arm06B  arm_UI
#define     arm06D  arm_UI
#define     arm06F  arm_UI
#define     arm079  arm_UI
#define     arm07B  arm_UI
#define     arm07D  arm_UI
#define     arm07F  arm_UI

#define     arm089  arm_UI
#define     arm08B  arm_UI
#define     arm08D  arm_UI
#define     arm08F  arm_UI
#define     arm099  arm_UI
#define     arm09B  arm_UI
#define     arm09D  arm_UI
#define     arm09F  arm_UI

#define     arm0A9  arm_UI
#define     arm0AB  arm_UI
#define     arm0AD  arm_UI
#define     arm0AF  arm_UI
#define     arm0B9  arm_UI
#define     arm0BB  arm_UI
#define     arm0BD  arm_UI
#define     arm0BF  arm_UI

#define     arm0C9  arm_UI
#define     arm0CB  arm_UI
#define     arm0CD  arm_UI
#define     arm0CF  arm_UI
#define     arm0D9  arm_UI
#define     arm0DB  arm_UI
#define     arm0DD  arm_UI
#define     arm0DF  arm_UI

#define     arm0E9  arm_UI
#define     arm0EB  arm_UI
#define     arm0ED  arm_UI
#define     arm0EF  arm_UI
#define     arm0F9  arm_UI
#define     arm0FB  arm_UI
#define     arm0FD  arm_UI
#define     arm0FF  arm_UI

#define     arm100  &CpuArm::execArm100_MsrCpsr
#define     arm120  &CpuArm::execArm120_MrsCpsrReg
#define     arm121  &CpuArm::execArm121_BX
#define     arm140  &CpuArm::execArm140_MsrSpsr
#define     arm160  &CpuArm::execArm160_MrsSpsrReg

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
#define     arm320  &CpuArm::execArm320_MrsCpsrImm
#define     arm340  arm_UI
#define     arm360  &CpuArm::execArm360_MrsSpsrImm
#define     armALU  &CpuArm::execALUInstruction
#define     armSTR  &CpuArm::execLdrStrInstruction
#define     armLDR  &CpuArm::execLdrStrInstruction
#define     armAxx  &CpuArm::execArmAxx_B
#define     armBxx  &CpuArm::execArmBxx_BL

#define     REPEAT_16(inst)     \
    inst, inst, inst, inst, inst, inst, inst, inst,     \
    inst, inst, inst, inst, inst, inst, inst, inst

#define     REPEAT256(inst)     \
    REPEAT_16(inst), REPEAT_16(inst), REPEAT_16(inst), REPEAT_16(inst),     \
    REPEAT_16(inst), REPEAT_16(inst), REPEAT_16(inst), REPEAT_16(inst),     \
    REPEAT_16(inst), REPEAT_16(inst), REPEAT_16(inst), REPEAT_16(inst),     \
    REPEAT_16(inst), REPEAT_16(inst), REPEAT_16(inst), REPEAT_16(inst)


//  ALU 命令のうち bit7-4 が 1xx1 であるものは注意する。    //
//  bit4 が 1 なのでシフト量を示すのにレジスタを使うが、    //
//  この時 bit7 は必ず  0 である。                          //
//  そこで bit7 が  1 になっている上記のビットパターンは    //
//  あり得ないので、別の命令に割り当てられている。          //

#define     INST_TABLE_000_1FF_ALU(CODE1, CODE2, ALU_OP)                    \
    armALU, armALU, armALU, armALU,     armALU, armALU, armALU, armALU,     \
    armALU, arm##CODE1##9, armALU, arm##CODE1##B,                           \
    armALU, arm##CODE1##D, armALU, arm##CODE1##F,                           \
    armALU, armALU, armALU, armALU,     armALU, armALU, armALU, armALU,     \
    armALU, arm##CODE2##9, armALU, arm##CODE2##B,                           \
    armALU, arm##CODE2##D, armALU, arm##CODE2##F

#define     INST_TABLE_ALU_TEST(CODE1, CODE2, ALU_OP)                       \
    REPEAT_16(arm##CODE1##0),                                               \
    armALU, armALU, armALU, armALU,     armALU, armALU, armALU, armALU,     \
    armALU, arm_UI, armALU, arm_UI,     armALU, arm_UI, armALU, arm_UI

#define     INST_TABLE_200_3FF(CODE1, CODE2, ALU_OP)                        \
    REPEAT_16(armALU),  REPEAT_16(armALU)

#define     INST_TABLE_400_7FF_STR_LDR(CODE1, CODE2)                        \
    REPEAT_16(armSTR),  REPEAT_16(armLDR)

//----------------------------------------------------------------
/**
**    命令テーブル。
**/

const   CpuArm::FnInst
CpuArm::s_armInstTable[4096] = {
    INST_TABLE_000_1FF_ALU(00, 01, AND),    //  00.0 -- 01.F
    INST_TABLE_000_1FF_ALU(02, 03, EOR),    //  02.0 -- 03.F
    INST_TABLE_000_1FF_ALU(04, 05, SUB),    //  04.0 -- 05.F
    INST_TABLE_000_1FF_ALU(06, 07, RSB),    //  06.0 -- 07.F
    INST_TABLE_000_1FF_ALU(08, 09, ADD),    //  08.0 -- 09.F
    INST_TABLE_000_1FF_ALU(0A, 0B, ADC),    //  0A.0 -- 0B.F
    INST_TABLE_000_1FF_ALU(0C, 0D, SBC),    //  0C.0 -- 0D.F
    INST_TABLE_000_1FF_ALU(0E, 0F, RSC),    //  0E.0 -- 0F.F

    //  10.0 -- 11.F
    INST_TABLE_ALU_TEST(10, 11, TST),       //  10.0 -- 11.F

//    INST_TABLE_ALU_TEST(12, 13, TEQ),       //  12.0 -- 13.F
    arm120, arm121, arm_UI, arm_UI,     arm_UI, arm_UI, arm_UI, arm_UI,
    arm_UI, arm_UI, arm_UI, arm_UI,     arm_UI, arm_UI, arm_UI, arm_UI,
    armALU, armALU, armALU, armALU,     armALU, armALU, armALU, armALU,
    armALU, arm_UI, armALU, arm_UI,     armALU, arm_UI, armALU, arm_UI,

    INST_TABLE_ALU_TEST(14, 15, CMP),       //  14.0 -- 15.F
    INST_TABLE_ALU_TEST(16, 17, CMN),       //  16.0 -- 17.F
    INST_TABLE_000_1FF_ALU(18, 19, ORR),    //  18.0 -- 19.F
    INST_TABLE_000_1FF_ALU(1A, 1B, MOV),    //  1A.0 -- 1B.F
    INST_TABLE_000_1FF_ALU(1C, 1D, BIC),    //  1C.0 -- 1D.F
    INST_TABLE_000_1FF_ALU(1E, 1F, MVN),    //  1E.0 -- 1F.F

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
    INST_TABLE_400_7FF_STR_LDR(40, 41),     //  40.0 -- 41.F
    INST_TABLE_400_7FF_STR_LDR(42, 43),     //  42.0 -- 43.F
    INST_TABLE_400_7FF_STR_LDR(44, 45),     //  44.0 -- 45.F
    INST_TABLE_400_7FF_STR_LDR(46, 47),     //  46.0 -- 47.F
    INST_TABLE_400_7FF_STR_LDR(48, 49),     //  48.0 -- 49.F
    INST_TABLE_400_7FF_STR_LDR(4A, 4B),     //  4A.0 -- 4B.F
    INST_TABLE_400_7FF_STR_LDR(4C, 4D),     //  4C.0 -- 4D.F
    INST_TABLE_400_7FF_STR_LDR(4E, 4F),     //  4E.0 -- 4F.F

    INST_TABLE_400_7FF_STR_LDR(50, 51),     //  50.0 -- 51.F
    INST_TABLE_400_7FF_STR_LDR(52, 53),     //  52.0 -- 53.F
    INST_TABLE_400_7FF_STR_LDR(54, 55),     //  54.0 -- 55.F
    INST_TABLE_400_7FF_STR_LDR(56, 57),     //  56.0 -- 57.F
    INST_TABLE_400_7FF_STR_LDR(58, 59),     //  58.0 -- 59.F
    INST_TABLE_400_7FF_STR_LDR(5A, 5B),     //  5A.0 -- 5B.F
    INST_TABLE_400_7FF_STR_LDR(5C, 5D),     //  5C.0 -- 5D.F
    INST_TABLE_400_7FF_STR_LDR(5E, 5F),     //  5E.0 -- 5F.F

    REPEAT256(arm_UI),      //  60.0 -- 6F.F
    REPEAT256(arm_UI),      //  70.0 -- 7F.F
    REPEAT256(arm_UI),      //  80.0 -- 8F.F
    REPEAT256(arm_UI),      //  90.0 -- 9F.F
    REPEAT256(armAxx),      //  A0.0 -- AF.F
    REPEAT256(armBxx),      //  B0.0 -- BF.F
    REPEAT256(arm_UI),      //  C0.0 -- CF.F
    REPEAT256(arm_UI),      //  D0.0 -- DF.F
    REPEAT256(arm_UI),      //  E0.0 -- EF.F
    REPEAT256(arm_UI),      //  F0.0 -- FF.F
};

#endif
