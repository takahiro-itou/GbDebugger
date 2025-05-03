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
**      An Interface of BaseDisCpu class.
**
**      @file       GbaMan/BaseDisCpu.h
**/

#if !defined( GBDEBUGGER_GBAMAN_INCLUDED_BASE_DIS_CPU_H )
#    define   GBDEBUGGER_GBAMAN_INCLUDED_BASE_DIS_CPU_H

#if !defined( GBDEBUGGER_COMMON_INCLUDED_DEBUGGER_TYPES_H )
#    include    "GbDebugger/Common/DebuggerTypes.h"
#endif

#include    <ostream>


GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {

//  クラスの前方宣言。  //
class   MemoryManager;


//========================================================================
//
//    Type Definitions.
//

struct  MnemonicMap  {
    OpeCode         mask;
    OpeCode         cval;
    const char *    mnemonic;
};

//========================================================================

CONSTEXPR_VAR   const  char  *  regNames[16] = {
    "R0" , "R1" , "R2" , "R3" , "R4" , "R5", "R6", "R7",
    "R8" , "R9" , "R10", "R11", "R12", "SP", "LR", "PC"
};

CONSTEXPR_VAR   const  char  *  conditions[16] = {
    ".EQ", ".NE", ".CS", ".CC", ".MI", ".PL", ".VS", ".VC",
    ".HI", ".LS", ".GE", ".LT", ".GT", ".LE", "",    ".NV"
};

//========================================================================
//
//    BaseDisCpu  class.
//

class  BaseDisCpu
{

//========================================================================
//
//    Constructor(s) and Destructor.
//
public:

    //----------------------------------------------------------------
    /**   インスタンスを初期化する
    **  （デフォルトコンストラクタ）。
    **
    **/
    BaseDisCpu();

    //----------------------------------------------------------------
    /**   インスタンスを破棄する
    **  （デストラクタ）。
    **
    **/
    virtual  ~BaseDisCpu();

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
public:

    //----------------------------------------------------------------
    /**   ニーモニックを表示する。
    **
    **  @param [in,out] outStr    出力ストリーム
    **  @param [in]     gmAddr    ゲスト上のアドレス
    **  @param [in]     opeCode   オペコード
    **/
    virtual  std::ostream  &
    writeMnemonic(
            std::ostream       &outStr,
            GuestMemoryAddress  gmAddr,
            const  OpeCode      opeCode)  const  = 0;

//========================================================================
//
//    Public Member Functions (Virtual Functions).
//

//========================================================================
//
//    Public Member Functions.
//

//========================================================================
//
//    Accessors.
//
public:

    void
    setMemoryManager(
            const  MemoryManager  & manMem)
    {
        this->m_pManMem = &manMem;
    }

//========================================================================
//
//    Protected Member Functions.
//

//========================================================================
//
//    For Internal Use Only.
//

//========================================================================
//
//    Member Variables.
//
protected:

    const   MemoryManager  *    m_pManMem;

//========================================================================
//
//    Other Features.
//
private:
    typedef     BaseDisCpu      This;
    BaseDisCpu          (const  This  &);
    This &  operator =  (const  This  &);
public:
    //  テストクラス。  //
    friend  class   BaseDisCpuTest;
};

//========================================================================
//
//    Inline Functions.
//

//----------------------------------------------------------------
/**
**
**/

inline  int
readMnemonicParameter(
        const  char *  &src,
        const  int      digMax)
{
    int val = 0;

    for ( int d = 0; d < digMax; ++ d ) {
        const  char  ch = (*(++ src));
        if ( ('0' <= ch) && (ch <= '9') ) {
            val = (val * 10) + (ch - '0');
        } else {
            --  src;
        }
    }

    return ( val );
}

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END

#endif
