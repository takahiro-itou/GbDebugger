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

//----------------------------------------------------------------
/**
**    命令テーブル。
**/

const   CpuThumb::FnInst
CpuThumb::s_thumbInstTable[256] = {
    nullptr,
};

#endif
