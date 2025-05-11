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
**      An Implementation of Thumb Instruction Table.
**
**      @file       GbaMan/ThumbInst.h
**/

#if !defined( GBDEBUGGER_GBAMAN_INCLUDED_THUMB_INST_H )
#    define   GBDEBUGGER_GBAMAN_INCLUDED_THUMB_INST_H


#define     thumbUI     nullptr

#define     thumb00     &CpuThumb::execBitShift<0>
#define     thumb08     &CpuThumb::execBitShift<1>
#define     thumb10     &CpuThumb::execBitShift<2>

//  ADD Rd, Rs, Rn/#nn
#define     thumb18     &CpuThumb::execOperateAddSub<0>
#define     thumb1A     &CpuThumb::execOperateAddSub<1>
#define     thumb1C     &CpuThumb::execOperateAddSub<2>
#define     thumb1E     &CpuThumb::execOperateAddSub<3>

//  MOV Rd, #nn
#define     thumb20     &CpuThumb::execOperateImm<0, 0>
#define     thumb21     &CpuThumb::execOperateImm<0, 1>
#define     thumb22     &CpuThumb::execOperateImm<0, 2>
#define     thumb23     &CpuThumb::execOperateImm<0, 3>
#define     thumb24     &CpuThumb::execOperateImm<0, 4>
#define     thumb25     &CpuThumb::execOperateImm<0, 5>
#define     thumb26     &CpuThumb::execOperateImm<0, 6>
#define     thumb27     &CpuThumb::execOperateImm<0, 7>

//  CMP Rd, #nn
#define     thumb28     &CpuThumb::execOperateImm<1, 0>
#define     thumb29     &CpuThumb::execOperateImm<1, 1>
#define     thumb2A     &CpuThumb::execOperateImm<1, 2>
#define     thumb2B     &CpuThumb::execOperateImm<1, 3>
#define     thumb2C     &CpuThumb::execOperateImm<1, 4>
#define     thumb2D     &CpuThumb::execOperateImm<1, 5>
#define     thumb2E     &CpuThumb::execOperateImm<1, 6>
#define     thumb2F     &CpuThumb::execOperateImm<1, 7>

//  ADD Rd, #nn
#define     thumb30     &CpuThumb::execOperateImm<2, 0>
#define     thumb31     &CpuThumb::execOperateImm<2, 1>
#define     thumb32     &CpuThumb::execOperateImm<2, 2>
#define     thumb33     &CpuThumb::execOperateImm<2, 3>
#define     thumb34     &CpuThumb::execOperateImm<2, 4>
#define     thumb35     &CpuThumb::execOperateImm<2, 5>
#define     thumb36     &CpuThumb::execOperateImm<2, 6>
#define     thumb37     &CpuThumb::execOperateImm<2, 7>

//  SUB Rd, #nn
#define     thumb38     &CpuThumb::execOperateImm<3, 0>
#define     thumb39     &CpuThumb::execOperateImm<3, 1>
#define     thumb3A     &CpuThumb::execOperateImm<3, 2>
#define     thumb3B     &CpuThumb::execOperateImm<3, 3>
#define     thumb3C     &CpuThumb::execOperateImm<3, 4>
#define     thumb3D     &CpuThumb::execOperateImm<3, 5>
#define     thumb3E     &CpuThumb::execOperateImm<3, 6>
#define     thumb3F     &CpuThumb::execOperateImm<3, 7>

#define     thumb40     &CpuThumb::execArithmeticLogic

//  High Register.
#define     thumb44     &CpuThumb::execOperateHighRegister<0>
#define     thumb45     &CpuThumb::execOperateHighRegister<1>
#define     thumb46     &CpuThumb::execOperateHighRegister<2>
#define     thumb47     &CpuThumb::execOperateHighRegister<3>

//  Store/Load
#define     thumb48     &CpuThumb::execStoreLoadRelative<15, 1>

//  STR  Rd, [Rb, Ro]
#define     thumb50     &CpuThumb::execStoreLoadWithRegOffset<0, BtWord>
//  STRH Rd, [Rb, Ro]
#define     thumb52     &CpuThumb::execStoreLoadWithRegOffset<0, uint16_t>
//  STRB Rd, [Rb, Ro]
#define     thumb54     &CpuThumb::execStoreLoadWithRegOffset<0, BtByte>
//  LDSB Rd, [Rb, Ro]
#define     thumb56     &CpuThumb::execStoreLoadWithRegOffset<3, BtByte>
//  LDR  Rd, [Rb, Ro]
#define     thumb58     &CpuThumb::execStoreLoadWithRegOffset<1, BtWord>
//  LDRH Rd, [Rb, Ro]
#define     thumb5A     &CpuThumb::execStoreLoadWithRegOffset<1, uint16_t>
//  LDRB Rd, [Rb, Ro]
#define     thumb5C     &CpuThumb::execStoreLoadWithRegOffset<1, BtByte>
//  LDSG Rd, [Rb, Ro]
#define     thumb5E     &CpuThumb::execStoreLoadWithRegOffset<3, int16_t>

#define     thumb60     &CpuThumb::execStoreLoadWithImmOffset<0, BtWord>
#define     thumb68     &CpuThumb::execStoreLoadWithImmOffset<1, BtWord>
#define     thumb70     &CpuThumb::execStoreLoadWithImmOffset<0, BtByte>
#define     thumb78     &CpuThumb::execStoreLoadWithImmOffset<1, BtByte>
#define     thumb80     &CpuThumb::execStoreLoadWithImmOffset<0, uint16_t>
#define     thumb88     &CpuThumb::execStoreLoadWithImmOffset<1, uint16_t>

#define     thumb90     &CpuThumb::execStoreLoadRelative<13, 0>
#define     thumb98     &CpuThumb::execStoreLoadRelative<13, 1>

//  ADD  Rd, PC, #nn
#define     thumbA0     &CpuThumb::execAddressingRelative<15>
//  ADD  Rd, SP, #nn
#define     thumbA8     &CpuThumb::execAddressingRelative<13>

//  ADD  SP, #nn
#define     thumbB0     &CpuThumb::execOperateSP

//  PUSH
#define     thumbB4     &CpuThumb::execPushPop<0, -1>
#define     thumbB5     &CpuThumb::execPushPop<0, 14>
#define     thumbBC     &CpuThumb::execPushPop<1, -1>
#define     thumbBD     &CpuThumb::execPushPop<1, 15>

#define     thumbC0     &CpuThumb::execMultipleStore<0>
#define     thumbC1     &CpuThumb::execMultipleStore<1>
#define     thumbC2     &CpuThumb::execMultipleStore<2>
#define     thumbC3     &CpuThumb::execMultipleStore<3>
#define     thumbC4     &CpuThumb::execMultipleStore<4>
#define     thumbC5     &CpuThumb::execMultipleStore<5>
#define     thumbC6     &CpuThumb::execMultipleStore<6>
#define     thumbC7     &CpuThumb::execMultipleStore<7>

#define     thumbC8     &CpuThumb::execMultipleLoad<0>
#define     thumbC9     &CpuThumb::execMultipleLoad<1>
#define     thumbCA     &CpuThumb::execMultipleLoad<2>
#define     thumbCB     &CpuThumb::execMultipleLoad<3>
#define     thumbCC     &CpuThumb::execMultipleLoad<4>
#define     thumbCD     &CpuThumb::execMultipleLoad<5>
#define     thumbCE     &CpuThumb::execMultipleLoad<6>
#define     thumbCF     &CpuThumb::execMultipleLoad<7>

#define     thumbD0     &CpuThumb::execConditionalBranch<0>
#define     thumbD1     &CpuThumb::execConditionalBranch<1>
#define     thumbD2     &CpuThumb::execConditionalBranch<2>
#define     thumbD3     &CpuThumb::execConditionalBranch<3>
#define     thumbD4     &CpuThumb::execConditionalBranch<4>
#define     thumbD5     &CpuThumb::execConditionalBranch<5>
#define     thumbD6     &CpuThumb::execConditionalBranch<6>
#define     thumbD7     &CpuThumb::execConditionalBranch<8>
#define     thumbD8     &CpuThumb::execConditionalBranch<9>
#define     thumbD9     &CpuThumb::execConditionalBranch<10>
#define     thumbDA     &CpuThumb::execConditionalBranch<11>
#define     thumbDB     &CpuThumb::execConditionalBranch<12>
#define     thumbDC     &CpuThumb::execConditionalBranch<13>
#define     thumbDD     &CpuThumb::execConditionalBranch<14>

#define     thumbF0     &CpuThumb::execBranchLinkLow<0>
#define     thumbF4     &CpuThumb::execBranchLinkLow<0xFF800000>
#define     thumbF8     &CpuThumb::execBranchLinkHigh


//----------------------------------------------------------------
/**
**    命令テーブル。
**/

const   CpuThumb::FnInst
CpuThumb::s_thumbInstTable[256] = {
    //  0x00 -- 0F  //
    thumb00, thumb00, thumb00, thumb00,   thumb00, thumb00, thumb00, thumb00,
    thumb08, thumb08, thumb08, thumb08,   thumb08, thumb08, thumb08, thumb08,

    //  0x10 -- 1F  //
    thumb10, thumb10, thumb10, thumb10,   thumb10, thumb10, thumb10, thumb10,
    thumb18, thumb18, thumb1A, thumb1A,   thumb1C, thumb1C, thumb1E, thumb1E,

    //  0x20 -- 2F  //
    thumb20, thumb21, thumb22, thumb23,   thumb24, thumb25, thumb26, thumb27,
    thumb28, thumb29, thumb2A, thumb2B,   thumb2C, thumb2D, thumb2E, thumb2F,

    //  0x30 -- 3F  //
    thumb30, thumb31, thumb32, thumb33,   thumb34, thumb35, thumb36, thumb37,
    thumb38, thumb39, thumb3A, thumb3B,   thumb3C, thumb3D, thumb3E, thumb3F,

    //  0x40 -- 4F  //
    thumb40, thumb40, thumb40, thumb40,   thumb44, thumb45, thumb46, thumb47,
    thumb48, thumb48, thumb48, thumb48,   thumb48, thumb48, thumb48, thumb48,

    //  0x50 -- 5F  //
    thumb50, thumb50, thumb52, thumb52,   thumb54, thumb54, thumb56, thumb56,
    thumb58, thumb58, thumb5A, thumb5A,   thumb5C, thumb5C, thumb5E, thumb5E,

    //  0x60 -- 6F  //
    thumb60, thumb60, thumb60, thumb60,   thumb60, thumb60, thumb60, thumb60,
    thumb68, thumb68, thumb68, thumb68,   thumb68, thumb68, thumb68, thumb68,

    //  0x70 -- 7F  //
    thumb70, thumb70, thumb70, thumb70,   thumb70, thumb70, thumb70, thumb70,
    thumb78, thumb78, thumb78, thumb78,   thumb78, thumb78, thumb78, thumb78,

    //  0x80 -- 8F  //
    thumb80, thumb80, thumb80, thumb80,   thumb80, thumb80, thumb80, thumb80,
    thumb88, thumb88, thumb88, thumb88,   thumb88, thumb88, thumb88, thumb88,

    //  0x90 -- 9F  //
    thumb90, thumb90, thumb90, thumb90,   thumb90, thumb90, thumb90, thumb90,
    thumb98, thumb98, thumb98, thumb98,   thumb98, thumb98, thumb98, thumb98,

    //  0xA0 -- AF  //
    thumbA0, thumbA0, thumbA0, thumbA0,   thumbA0, thumbA0, thumbA0, thumbA0,
    thumbA8, thumbA8, thumbA8, thumbA8,   thumbA8, thumbA8, thumbA8, thumbA8,

    //  0xB0 -- BF  //
    thumbB0, thumbUI, thumbUI, thumbUI,   thumbB4, thumbB5, thumbUI, thumbUI,
    thumbUI, thumbUI, thumbUI, thumbUI,   thumbBC, thumbBD, nullptr, thumbUI,

    //  0xC0 -- CF  //
    thumbC0, thumbC1, thumbC2, thumbC3,   thumbC4, thumbC5, thumbC6, thumbC7,
    thumbC8, thumbC9, thumbCA, thumbCB,   thumbCC, thumbCD, thumbCE, thumbCF,

    //  0xD0 -- DF  //
    thumbD0, thumbD1, thumbD2, thumbD3,   thumbD4, thumbD5, thumbD6, thumbD7,
    thumbD8, thumbD9, thumbDA, thumbDB,   thumbDC, thumbDD, nullptr, nullptr,

    //  0xE0 -- EF  //
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,

    //  0xF0 -- FF  //
    thumbF0, thumbF0, thumbF0, thumbF0,   thumbF4, thumbF4, thumbF4, thumbF4,
    thumbF8, thumbF8, thumbF8, thumbF8,   thumbF8, thumbF8, thumbF8, thumbF8,
};

#endif
