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

#include    "CpuArm/CpuArm.h"
#include    "CpuArm/DisArm.h"

#include    <ostream>
#include    <stdio.h>
#include    <sys/stat.h>


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

namespace  {

const char * regs[16] = {
    "R0" , "R1" , "R2" , "R3" , "R4" , "R5", "R6", "R7",
    "R8" , "R9" , "R10", "R11", "R12", "SP", "LR", "PC"
};

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
    : m_manMem(),
      m_cpuArm(nullptr),
      m_regs()
{
    this->m_cpuArm  = new CpuArm(this->m_manMem);
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
        std::ostream       &outStr,
        GuestMemoryAddress  gmAddr)
{
    const  OpeCode  opeCode = readMemory<uint32_t>(gmAddr);

    DisArm  dis;
    return  dis.writeMnemonic(outStr, gmAddr, opeCode);
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
    this->m_manMem.allocateMemory();
    this->m_manMem.buildMemoryTable();

    //  ROM の内容を読み込む。  **/
    const   size_t  cbRead  = (stbuf.st_size < MEM_SIZE_ROM ?
                               stbuf.st_size : MEM_SIZE_ROM);
    FILE *  fp  = fopen(szFileName, "rb");
    if ( fp == nullptr ) {
        this->closeInstance();
        return ( ErrCode::FILE_IO_ERROR );
    }

    LpByteWriteBuf  memRom  = this->m_manMem.getHostAddressOfGuestRom();
    size_t  retRead = fread(memRom, sizeof(uint8_t), cbRead, fp);
    GBDEBUGGER_UNUSED_VAR(retRead);

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
