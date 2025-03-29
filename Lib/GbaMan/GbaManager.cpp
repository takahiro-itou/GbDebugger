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
**      An Implementation of GbaManager class.
**
**      @file       GbaMan/GbaManager.cpp
**/

#include    "GbDebugger/GbaMan/GbaManager.h"

#include    <ostream>
#include    <stdio.h>
#include    <sys/stat.h>


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

namespace  {

struct  Opecodes  {
    uint32_t    mask;
    uint32_t    cval;
    const char *    mnemonic;
};

const char * regs[16] = {
    "R0" , "R1" , "R2" , "R3" , "R4" , "R5", "R6", "R7",
    "R8" , "R9" , "R10", "R11", "R12", "SP", "LR", "PC"
};

const char * conditions[16] = {
    "EQ", "NE", "CS", "CC", "MI", "PL", "VS", "VC",
    "HI", "LS", "GE", "LT", "GT", "LE", "AL", "NV"
};

const Opecodes armOpecodes[] = {
    //  Branch
    { 0x0FF000F0, 0x01200010, "BX.%c %r0" },
    { 0x0F000000, 0x0A000000, "B.%c %o" },
    { 0x0F000000, 0x0B000000, "BL.%c %o" },
    { 0x0F000000, 0x0F000000, "SWI.%c %q" }
};

}   //  End of (Unnamed) namespace.

inline  void *
getMemoryAddress(
        const  MemoryTable (&map)[256],
        const  uint32_t     addr)
{
    const uint32_t  pg  = (addr >> 24);
    const uint32_t  ofs = addr & (map[pg].mask);
    return ( map[pg].address + ofs );
}

template <typename T>
inline  const  T
readMemory(
        const  MemoryTable (&map)[256],
        const  uint32_t     addr)
{
    const T  *  ptr = static_cast<const T *>(getMemoryAddress(map, addr));
    return ( *ptr );
}

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
    : m_memBios(nullptr),
      m_memWorkRam(nullptr),
      m_memInternalRam(nullptr),
      m_memPaletteRam (nullptr),
      m_memIO(nullptr),
      m_memVRam(nullptr),
      m_memOam (nullptr),
      m_memRom (nullptr),
      m_memSave(nullptr)
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
//    ニーモニックを表示する。
//

std::ostream  &
GbaManager::disassembleArm(
        std::ostream  & outStr,
        const uint32_t  addr)
{
    char    buf[256];
    uint32_t    opecode = readMemory<uint32_t>(this->m_tblMem, addr);

    const Opecodes * oc = armOpecodes;
    for ( ; (opecode & oc->mask) != oc->cval; ++ oc ) ;

    sprintf(buf, "%08x %08x ", addr, opecode);
    outStr  <<  buf;

    int             reg_id  = 0;
    size_t          len = 0;
    const  char  *  src = oc->mnemonic;
    char  *         dst = buf;

    while (*src) {
        len = 0;
        if ( *src != '%' ) {
            *(dst ++)   = *(src ++);
        } else {
            ++  src;
            switch ( *src ) {
            case  'c':
                len = sprintf(dst, "%s", conditions[opecode >> 28]);
                break;
            case  'r':
                reg_id  = (opecode >> ((*(++ src) - '0') * 4)) & 15;
                len = sprintf(dst, "%s", regs[reg_id]);
                break;
            case  'o':  {
                * (dst ++)  = '$';
                int off = opecode & 0x00FFFFFF;
                if ( off & 0x00800000 ) {
                    off |= 0xFF000000;
                }
                off <<= 2;
                len = sprintf(dst, "%08x", addr + 8 + off);
                }
                break;
            }
            ++  src;
        }
        dst += len;
    }

    *(dst ++)   = '\0';
    outStr  <<  buf;

    return ( outStr );
}

//----------------------------------------------------------------
//    ニーモニックを表示する。
//

std::ostream  &
GbaManager::disassembleThumb(
        std::ostream  & outStr,
        const uint32_t  addr)
{
    return ( outStr );
}

//----------------------------------------------------------------
//    リセットを行う。
//

ErrCode
GbaManager::doHardReset()
{
    for ( int i = 0; i < 16; ++ i ) {
        this->m_regs[ i].dw = 0x00000000;
    }
    this->m_regs[15].dw = 0x08000000;

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
    this->m_memWorkRam      = new uint8_t[MEM_SIZE_WRAM];
    this->m_memRom          = new uint8_t[MEM_SIZE_ROM];

    this->m_memBios         = new uint8_t[MEM_SIZE_BIOS];
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

    //  ROM の内容を読み込む。  **/
    const   size_t  cbRead  = (stbuf.st_size < MEM_SIZE_ROM ?
                               stbuf.st_size : MEM_SIZE_ROM);
    FILE *  fp  = fopen(szFileName, "rb");
    if ( fp == nullptr ) {
        this->closeInstance();
        return ( ErrCode::FILE_IO_ERROR );
    }

    fread(this->m_memRom, sizeof(uint8_t), cbRead, fp);
    fclose(fp);

    return ( ErrCode::SUCCESS );
}

//----------------------------------------------------------------
//    レジスタの内容をダンプする。
//

std::ostream  &
GbaManager::printRegisters(
        std::ostream  & outStr)  const
{
    char    buf[256];

    for ( int i = 0; i < 16; ++ i ) {
        sprintf(buf, "%3s: %08x ", regs[i], this->m_regs[i].dw);
        outStr  <<  buf;
        if ( (i & 3) == 3 ) {
            outStr  <<  std::endl;
        }
    }

    return ( outStr );
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
