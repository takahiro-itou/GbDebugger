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
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,

    //  0x20 -- 2F  //
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,

    //  0x30 -- 3F  //
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,

    //  0x40 -- 4F  //
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
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
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
};

#endif
