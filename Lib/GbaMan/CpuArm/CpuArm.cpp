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
**      An Implementation of CpuArm class.
**
**      @file       GbaMan/CpuArm.cpp
**/

#include    "CpuArm.h"

#include    "GbDebugger/GbaMan/GbaManager.h"

#include    <ostream>


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
    { 0x0FF000F0, 0x01200010, "BX.%c\t%r0" },
    { 0x0F000000, 0x0A000000, "B.%c\t%o" },
    { 0x0F000000, 0x0B000000, "BL.%c\t%o" },
    { 0x0F000000, 0x0F000000, "SWI.%c\t%q" }
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


//----------------------------------------------------------------
//    ニーモニックを表示する。
//

std::ostream  &
GbaManager::disassembleArm(
        std::ostream  & outStr,
        const uint32_t  addr)
{
    char    buf[256] = { 0 };
    uint32_t    opecode = readMemory<uint32_t>(addr);

    const Opecodes * oc = armOpecodes;
    for ( ; (opecode & oc->mask) != oc->cval; ++ oc ) ;

    sprintf(buf, "%08x:   %08x\t", addr, opecode);
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
                len = sprintf(dst, "%08x ; (%08x)", off, addr + 8 + off);
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


}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END
