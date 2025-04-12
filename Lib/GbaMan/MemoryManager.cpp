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
**      An Implementation of MemoryManager class.
**
**      @file       GbaMan/MemoryManager.cpp
**/

#include    "GbDebugger/GbaMan/MemoryManager.h"


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

namespace  {

}   //  End of (Unnamed) namespace.


//========================================================================
//
//    MemoryManager  class.
//

//========================================================================
//
//    Constructor(s) and Destructor.
//

//----------------------------------------------------------------
//    インスタンスを初期化する
//  （デフォルトコンストラクタ）。

MemoryManager::MemoryManager()
    : m_memBios(nullptr),
      m_memWRam(nullptr),
      m_memIRam(nullptr),
      m_memIO  (nullptr),
      m_memPRam(nullptr),
      m_memVRam(nullptr),
      m_memOam (nullptr),
      m_memRom (nullptr),
      m_memSave(nullptr),
      m_tblMem()
{
}

//----------------------------------------------------------------
//    インスタンスを破棄する
//  （デストラクタ）。
//

MemoryManager::~MemoryManager()
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
//    メモリを確保する。
//

ErrCode
MemoryManager::allocateMemory()
{
    this->m_memBios = new BtByte [MEM_SIZE_BIOS];
    this->m_memWRam = new BtByte [MEM_SIZE_WRAM];
    this->m_memIRam = new BtByte [MEM_SIZE_IRAM];
    this->m_memIO   = new BtByte [MEM_SIZE_IOMEM];
    this->m_memPRam = new BtByte [MEM_SIZE_PRAM];
    this->m_memVRam = new BtByte [MEM_SIZE_VRAM];
    this->m_memOam  = new BtByte [MEM_SIZE_OAM];
    this->m_memRom  = new BtByte [MEM_SIZE_ROM];
    this->m_memSave = new BtByte [MEM_SIZE_SRAM];

    return ( ErrCode::SUCCESS );
}

//----------------------------------------------------------------
//    メモリを解放する。
//

ErrCode
MemoryManager::releaseMemory()
{
    delete []   this->m_memBios;    this->m_memBios = nullptr;
    delete []   this->m_memWRam;    this->m_memWRam = nullptr;
    delete []   this->m_memIRam;    this->m_memIRam = nullptr;
    delete []   this->m_memIO;      this->m_memIO   = nullptr;
    delete []   this->m_memPRam;    this->m_memPRam = nullptr;
    delete []   this->m_memVRam;    this->m_memVRam = nullptr;
    delete []   this->m_memOam;     this->m_memOam  = nullptr;
    delete []   this->m_memRom;     this->m_memRom  = nullptr;
    delete []   this->m_memSave;    this->m_memSave = nullptr;

    return ( ErrCode::SUCCESS );
}

//========================================================================
//
//    Public Member Functions.
//

//----------------------------------------------------------------
//    メモリアドレスを計算する。
//

LpWriteBuf
MemoryManager::getMemoryAddress(
        const   GuestMemoryAddress  gmAddr)  const
{
    return ( nullptr );
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
