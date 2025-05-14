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
**      An Implementation of DisUtils.
**
**      @file       GbaMan/Utils/DisUtils.inl
**/

#if !defined( GBDEBUGGER_GBAMAN_UTILS_INCLUDED_DIS_UTILS_INL )
#    define   GBDEBUGGER_GBAMAN_UTILS_INCLUDED_DIS_UTILS_INL

GBDEBUGGER_NAMESPACE_BEGIN
namespace  GbaMan  {


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
        const  char  ch = (*(src ++));
        if ( ('0' <= ch) && (ch <= '9') ) {
            val = (val * 10) + (ch - '0');
        } else {
            --  src;
            break;
        }
    }

    return ( val );
}

//----------------------------------------------------------------

inline  OpeCode
getOpeCodeMask(
        const  char *  &src,
        const  int      digMax)
{
    if ( (* src) == 'm' ) {
        ++  src;
        //  パラメータでマスクを直接指定する。  //
        return  static_cast<OpeCode>(readMnemonicParameter(src, digMax));
    }

    //  パラメータはマスクのビット数を指定。    //
    const  int  immMask = readMnemonicParameter(src, 2);
    return  ( static_cast<OpeCode>(1 << immMask) - 1 );
}

//----------------------------------------------------------------
/**   スケール付き（符号付き）即値。
**
**  src から次の情報を読みだす。{B,M,S}
**  次に opeCode の下位 B ビット目より上を取り出し、
**  マスク M と左シフト S を適用する。
**  具体的には ((opeCode >> B) & M) << S  を計算する。
**
**  @param [in]     opeCode
**  @param [in,out] src,
**  @return
**/

inline  RegType
getSignedScaleImmediate(
        const  OpeCode  opeCode,
        const  char *  &src)
{
    RegType val = 0;

    if ( *(src) == '{' ) {
        ++  src;
        const  int      immBit  = readMnemonicParameter(src, 2);
        ++  src;    //  カンマを読み捨て。      //
        const  int      digMask = readMnemonicParameter(src, 2) - 1;
        const  OpeCode  sgnMask = static_cast<OpeCode>(1 << digMask);
        const  OpeCode  immMask = (sgnMask - 1) | sgnMask;
        ++  src;    //  カンマを読み捨て。
        const  int      immSft  = readMnemonicParameter(src, 2);
        ++  src;    //  末尾の }  を読み捨て。  //

        val = ((opeCode >> immBit) & immMask);
        if ( (val & sgnMask) ) {
            //  符号拡張する。  //
            val |= ~immMask;
        }
        val <<= immSft;
    }

    return ( val );
}

//----------------------------------------------------------------
/**   スケール付き即値。
**
**  src から次の情報を読みだす。{B,M,S}
**  次に opeCode の下位 B ビット目より上を取り出し、
**  マスク M と左シフト S を適用する。
**  具体的には ((opeCode >> B) & M) << S  を計算する。
**
**  @param [in]     opeCode
**  @param [in,out] src,
**  @return
**/

inline  RegType
getUnsignedScaleImmediate(
        const  OpeCode  opeCode,
        const  char *  &src)
{
    RegType val = 0;

    if ( *(src) == '{' ) {
        ++  src;
        const  int  immBit  = readMnemonicParameter(src, 2);
        ++  src;    //  カンマを読み捨て。      //
        const  int  immMask = getOpeCodeMask(src, 8);
        ++  src;    //  カンマを読み捨て。
        const  int  immSft  = readMnemonicParameter(src, 2);
        ++  src;    //  末尾の }  を読み捨て。  //
        val = ((opeCode >> immBit) & immMask) << immSft;
    }

    return ( val );
}

}   //  End of namespace  GbaMan
GBDEBUGGER_NAMESPACE_END

#endif
