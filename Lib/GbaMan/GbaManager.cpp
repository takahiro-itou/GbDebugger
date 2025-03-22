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
**      An Implementation of SampleDocument class.
**
**      @file       GbaMan/GbaManager.cpp
**/

#include    "GbDebugger/GbaMan/GbaManager.h"

#include    <stdio.h>
#include    <sys/stat.h>


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

namespace  {

}   //  End of (Unnamed) namespace.


//========================================================================
//
//    GbaManager  class.
//

//========================================================================
//
//    Constructor(s) and Destructor.
//

//----------------------------------------------------------------
//    インスタンスを初期化する
//  （デフォルトコンストラクタ）。

GbaManager::GbaManager()
{
}

//----------------------------------------------------------------
//    インスタンスを破棄する
//  （デストラクタ）。
//

GbaManager::~GbaManager()
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
//    現在動作しているインスタンスを閉じる。
//

ErrCode
GbaManager::closeInstance()
{
    return ( ErrCode::SUCCESS );
}

//----------------------------------------------------------------
//    ROM ファイルを読み込む。
//

ErrCode
GbaManager::openRomFile(
        const   char *  szFileName)
{
    struct stat stbuf;

    //  ファイルの情報を取得する。  //
    int rc  = stat(szFileName, &stbuf);
    if ( rc < 0 ) {
        perror("open rom file");
        return ( ErrCode::FILE_OPEN_ERROR );
    }

    //  ファイルサイズが 0xC0 (192) バイト未満の時は、  //
    //  必要なヘッダが存在していないのでエラーにする。  //
    if ( stbuf.st_size <= 192 ) {
        return ( ErrCode::FILE_IO_ERROR );
    }

    //  メモリの各領域を確保して、テーブルに保管する。  //
    this->m_memWorkRam  = new uint8_t[MEM_SIZE_WRAM];
    this->m_memRom      = new uint8_t[MEM_SIZE_ROM];

    this->m_memBios     = new uint8_t[MEM_SIZE_BIOS];
    this->m_memInternalRam  = new uint8_t[MEM_SIZE_IRAM];
    this->m_memIO           = new uint8_t[MEM_SIZE_IOMEM];
    this->m_memPaletteRam   = new uint8_t[MEM_SIZE_PRAM];
    this->m_memVRam         = new uint8_t[MEM_SIZE_VRAM];
    this->m_memOam          = new uint8_t[MEM_SIZE_OAM];
    this->m_memSave         = new uint8_t[MEM_SIZE_SRAM];

    for ( int i = 0; i < 256; ++ i ) {
        this->m_tblMem[i].address   = nullptr;
        this->m_tblMem[i].mask      = 0x00000000;
    }

    this->m_tblMem[ 0].address  = this->m_memBios;
    this->m_tblMem[ 2].address  = this->m_memWorkRam;
    this->m_tblMem[ 3].address  = this->m_memInternalRam;
    this->m_tblMem[ 4].address  = this->m_memIO;
    this->m_tblMem[ 5].address  = this->m_memPaletteRam;
    this->m_tblMem[ 6].address  = this->m_memVRam;
    this->m_tblMem[ 7].address  = this->m_memOam;

    this->m_tblMem[ 8].address  = this->m_memRom;
    this->m_tblMem[ 9].address  = this->m_memRom;
    this->m_tblMem[10].address  = this->m_memRom;
    this->m_tblMem[11].address  = this->m_memRom;
    this->m_tblMem[12].address  = this->m_memRom;
    this->m_tblMem[13].address  = this->m_memRom;

    this->m_tblMem[14].address  = this->m_memSave;

    this->m_tblMem[ 0].mask     = MEM_MASK_BIOS;
    this->m_tblMem[ 2].mask     = MEM_MASK_WRAM;
    this->m_tblMem[ 3].mask     = MEM_MASK_IRAM;
    this->m_tblMem[ 4].mask     = MEM_MASK_IOMEM;
    this->m_tblMem[ 5].mask     = MEM_MASK_PRAM;
    this->m_tblMem[ 6].mask     = MEM_MASK_VRAM;
    this->m_tblMem[ 7].mask     = MEM_MASK_OAM;
    this->m_tblMem[ 8].mask     = MEM_MASK_ROM;
    this->m_tblMem[ 9].mask     = MEM_MASK_ROM;
    this->m_tblMem[10].mask     = MEM_MASK_ROM;
    this->m_tblMem[11].mask     = MEM_MASK_ROM;
    this->m_tblMem[12].mask     = MEM_MASK_ROM;
    this->m_tblMem[13].mask     = MEM_MASK_ROM;
    this->m_tblMem[14].mask     = MEM_MASK_SRAM;

    for ( int i = 0; i < 256; ++ i ) {
        this->m_tblMem[i].size  = (this->m_tblMem[i].mask) + 1;
    }

    return ( ErrCode::FAILURE );
}

//========================================================================
//
//    Public Member Functions.
//

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

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END
