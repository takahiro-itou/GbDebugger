dnl ----------------------------------------------------------------
dnl
dnl   Linker Scripts.
dnl   リンカスクリプトを生成する。
dnl

AC_CONFIG_FILES([Lib/libgbdCommon.a:Lib/.LinkerScript/Common.in])
AC_CONFIG_FILES([Lib/libgbdGbaMan.a:Lib/.LinkerScript/Gba.in])
AC_CONFIG_FILES([Lib/libgbdImages.a:Lib/.LinkerScript/Images.in])

