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
**      Operator Constants.
**
**      @file       GbaMan/OperatorConst.h
**/

#if !defined( GBDEBUGGER_GBAMAN_CPUARM_INCLUDED_OPERATOR_CONST_H )
#    define   GBDEBUGGER_GBAMAN_CPUARM_INCLUDED_OPERATOR_CONST_H


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

//----------------------------------------------------------------
/**
**    演算の種類。
**/

enum class  AluOp  {
    AND = 0x00,
    EOR = 0x01,
    SUB = 0x02,
    RSB = 0x03,
    ADD = 0x04,
    ADC = 0x05,
    SBC = 0x06,
    RSC = 0x07,
    TST = 0x08,
    TEQ = 0x09,
    CMP = 0x0A,
    CMN = 0x0B,
    ORR = 0x0C,
    MOV = 0x0D,
    BIC = 0x0E,
    MVN = 0x0F,
};

//----------------------------------------------------------------
/**
**    演算の種類
**/

enum class  MulOp  {
    MUL     = 0x00,
    MLA     = 0x01,
    UMAAL   = 0x02,     /**<  未サポート。  **/
    UNDEF_3 = 0x03,     /**<  未定義命令。  **/
    UMULL   = 0x04,
    UMLAL   = 0x05,
    SMULL   = 0x06,
    SMLAL   = 0x07,
    SMLAxy  = 0x08,     /**<  未サポート。  **/
    SMLAWy  = 0x09,     /**<  未サポート。  **/
    SMULWy  = 0x09,     /**<  未サポート。  **/
    SMLALxy = 0x0A,     /**<  未サポート。  **/
    SMULxy  = 0x0B,     /**<  未サポート。  **/
    UNDEF_C = 0x0C,     /**<  未定義命令。  **/
    UNDEF_D = 0x0D,     /**<  未定義命令。  **/
    UNDEF_E = 0x0E,     /**<  未定義命令。  **/
    UNDEF_F = 0x0F,     /**<  未定義命令。  **/
};

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END

#endif
