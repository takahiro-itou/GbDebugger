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

#define     thumb48     &CpuThumb::execStoreLoadRelative<15, 1>

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
    thumb40, thumb40, thumb40, thumb40,   nullptr, nullptr, nullptr, nullptr,
    thumb48, thumb48, thumb48, thumb48,   thumb48, thumb48, thumb48, thumb48,

    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,

    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,

    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,

    //  0x80 -- 8F  //
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,

    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,

    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,

    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,

    //  0xC0 -- CF  //
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,

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
