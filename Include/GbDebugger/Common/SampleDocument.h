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
**      An Interface of SampleDocument class.
**
**      @file       Common/SampleDocument.h
**/

#if !defined( GBDEBUGGER_COMMON_INCLUDED_SAMPLE_DOCUMENT_H )
#    define   GBDEBUGGER_COMMON_INCLUDED_SAMPLE_DOCUMENT_H

#if !defined( GBDEBUGGER_COMMON_INCLUDED_DEBUGGER_TYPES_H )
#    include    "DebuggerTypes.h"
#endif

#if !defined( GBDEBUGGER_SYS_STL_INCLUDED_STRING )
#    include    <string>
#    define   GBDEBUGGER_SYS_STL_INCLUDED_STRING
#endif

GBDEBUGGER_NAMESPACE_BEGIN
namespace  Common  {

//  クラスの前方宣言。  //


//========================================================================
//
//    SampleDocument  class.
//

class  SampleDocument
{

//========================================================================
//
//    Internal Type Definitions.
//

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
    SampleDocument();

    //----------------------------------------------------------------
    /**   インスタンスを破棄する
    **  （デストラクタ）。
    **
    **/
    virtual  ~SampleDocument();

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
public:

    //----------------------------------------------------------------
    /**   入力メッセージ中に含まれるアルファベットを数える。
    **
    **  @return     半角アルファベット [A-Za-z] の文字数
    **/
    virtual  int
    countAlphabet()  const;

//========================================================================
//
//    Public Member Functions.
//

//========================================================================
//
//    Accessors.
//
public:

    //----------------------------------------------------------------
    /**   メッセージを設定する。
    **
    **  @param [in] message   入力データ
    **  @return     void.
    **/
    void
    setMessage(
            const  std::string  &message);

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
private:

    std::string     m_message;

//========================================================================
//
//    Other Features.
//
public:
    //  テストクラス。  //
    friend  class   SampleDocumentTest;
};

}   //  End of namespace  Common
GBDEBUGGER_NAMESPACE_END

#endif
