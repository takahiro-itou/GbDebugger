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
#include    "CpuThumb/CpuThumb.h"

#include    "GbDebugger/Common/DebuggerUtils.h"

#include    <ostream>
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
    : m_manMem(),
      m_cpuCur (nullptr),
      m_cpuMod0(nullptr),
      m_cpuMod1(nullptr),
      m_cpuMode(0)
{
    this->m_cpuMod0 = new CpuArm(*this, this->m_manMem);
    this->m_cpuMod1 = new CpuThumb(*this, this->m_manMem);
    this->m_cpuCur  = this->m_cpuMod0;
}

//----------------------------------------------------------------
//    インスタンスを破棄する
//  （デストラクタ）。
//

GbaManager::~GbaManager()
{
    delete  this->m_cpuMod0;
    this->m_cpuMod0 = nullptr;

    delete  this->m_cpuMod1;
    this->m_cpuMod1 = nullptr;
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
//    リセットを行う。
//

ErrCode
GbaManager::doHardReset()
{
    this->m_cpuCur  = this->m_cpuMod0;
    this->m_cpuCur->doHardReset();

    return ( ErrCode::SUCCESS );
}

//----------------------------------------------------------------
//    現在の命令を実行する。
//

InstExecResult
GbaManager::executeCurrentInst()
{
    return  this->m_cpuCur->executeNextInst();
}

//----------------------------------------------------------------
//    プログラムカウンタを取得する。
//

GuestMemoryAddress
GbaManager::getNextPC()  const
{
    return  this->m_cpuCur->getNextPC();
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
    return  this->m_cpuCur->printRegisters(outStr);
}

//----------------------------------------------------------------
//    ニーモニックを表示する。
//

std::ostream  &
GbaManager::disassembleArm(
        std::ostream       &outStr,
        GuestMemoryAddress  gmAddr)
{
    const  OpeCode  opeCode = readMemory<OpeCode>(gmAddr);

    DisArm  dis;
    return  dis.writeMnemonic(outStr, gmAddr, opeCode);
}

//----------------------------------------------------------------
//    ニーモニックを表示する。
//

std::ostream  &
GbaManager::writeMnemonicCurrent(
        std::ostream       &outStr,
        GuestMemoryAddress  gmAddr)  const
{
    return ( outStr );
}

//----------------------------------------------------------------
//    ニーモニックを表示する。
//

std::ostream  &
GbaManager::disassembleThumb(
        std::ostream       &outStr,
        GuestMemoryAddress  gmAddr)
{
    return ( outStr );
}

//========================================================================
//
//    Public Member Functions.
//

//----------------------------------------------------------------
//    CPU モードを切り替える。
//

ErrCode
GbaManager::changeCpuMode(
        const  RegType  thumbState)
{
    //  現在のモードの状態を保存する。  //
    RegBank regs;
    this->m_cpuCur->getRegisters(regs);

    if ( (this->m_cpuMode = (thumbState & CPSR::FLAG_T)) ) {
        //  THUMB モード。  //
        this->m_cpuCur  = this->m_cpuMod1;
    } else {
        //  ARM モード。    //
        this->m_cpuCur  = this->m_cpuMod0;
    }

    this->m_cpuCur->setRegisters(regs);

    return ( ErrCode::SUCCESS );
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

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END
