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
**      An Interface of MemoryTable structure.
**
**      @file       GbaMan/MemoryTable.h
**/

#if !defined( GBDEBUGGER_GBAMAN_INCLUDED_MEMORY_TABLE_H )
#    define   GBDEBUGGER_GBAMAN_INCLUDED_MEMORY_TABLE_H

#if !defined( GBDEBUGGER_COMMON_INCLUDED_DEBUGGER_TYPES_H )
#    include    "GbDebugger/Common/DebuggerTypes.h"
#endif


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

//  クラスの前方宣言。  //


//========================================================================
//
//    MemoryTable  structure.
//

struct  MemoryTable
{
    uint8_t *   address;
    uint32_t    mask;
};

//========================================================================
//
//    Memory Size & Mask.
//

enum  {
    /**   0x00000000-0x00003FFF :  16 KiB : BIOS.    **/
    MEM_SIZE_BIOS   = 0x00004000,
    MEM_MASK_BIOS   = 0x00003FFF,   /**<  アドレスのマスク用。  **/

    /**   0x02000000-0x0203FFFF : 256 KiB : WRAM.   **/
    MEM_SIZE_WRAM   = 0x00040000,
    MEM_MASK_WRAM   = 0x0003FFFF,   /**<  アドレスのマスク用。  **/

    /**   0x03000000-0x03007FFF :  32 KiB : IRAM.   **/
    MEM_SIZE_IRAM   = 0x00008000,
    MEM_MASK_IRAM   = 0x00007FFF,   /**<  アドレスのマスク用。  **/

    /**   0x04000000-0x040003FE :   1 KiB : I/O.    **/
    MEM_SIZE_IOMEM  = 0x00000400,
    MEM_MASK_IOMEM  = 0x000003FF,   /**<  アドレスのマスク用。  **/

    /**   0x05000000-0x050003FF :   1 KiB : パレット。  **/
    MEM_SIZE_PRAM   = 0x00000400,
    MEM_MASK_PRAM   = 0x000003FF,   /**<  アドレスのマスク用。  **/

    /**   0x06000000-0x06017FFF :  96 KiB : VRAM.   **/
    MEM_SIZE_VRAM   = 0x00020000,
    MEM_MASK_VRAM   = 0x0001FFFF,   /**<  アドレスのマスク用。  **/

    /**   0x07000000-0x070003FF :   1 KiB : OAM.    **/
    MEM_SIZE_OAM    = 0x000000400,
    MEM_MASK_OAM    = 0x0000003FF,  /**<  アドレスのマスク用。  **/

    /**   0x08000000-0x09FFFFFF :  32 MiB : ROM.    **/
    MEM_SIZE_ROM    = 0x02000000,
    MEM_MASK_ROM    = 0x01FFFFFF,   /**<  アドレスのマスク用。  **/
}

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END

#endif
