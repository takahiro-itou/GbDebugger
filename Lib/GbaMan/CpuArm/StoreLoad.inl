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
**      An Implementation of Store/Load Instructions.
**
**      @file       GbaMan/CpuArm/StoreLoad.inl
**/

#if !defined( GBDEBUGGER_GBAMAN_CPUARM_INCLUDED_STORE_LOAD_INL )
#    define   GBDEBUGGER_GBAMAN_CPUARM_INCLUDED_STORE_LOAD_INL

#if !defined( GBDEBUGGER_GBAMAN_INCLUDED_CPU_ARM_H )
#    include    "CpuArm.h"
#endif


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

namespace  {

}   //  End of (Unnamed) namespace.


//========================================================================
//
//    Store/Load  Instructions.
//

//----------------------------------------------------------------

template  <int I, int P, int U, typename B,
           int BIT21, int OP, typename SHIFTOP>
GBD_REGPARM     InstExecResult
CpuArm::execStoreLoad(
        const  OpeCode  opeCode)
{
    std::cerr   <<  "Not Implemented (StoreLoad With RegOffs)"  <<  std::endl;
    return ( InstExecResult::UNDEFINED_OPECODE );
}

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END

#endif
