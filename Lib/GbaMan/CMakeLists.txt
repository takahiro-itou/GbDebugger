
##----------------------------------------------------------------
##
##    モジュールの設定。
##

set(module_title        "Gba")

Include (${common_cmake_dir}/BuildCommon/ConfigModule.cmake)
Include (${common_cmake_dir}/BuildCommon/ConfigTarget.cmake)

##----------------------------------------------------------------
##
##    ソースファイルとヘッダファイルのリスト。
##

set(library_source_files
)

set(inclib_header_files
)

##----------------------------------------------------------------
##
##    ビルドとインストールの設定。
##

Include (${common_cmake_dir}/BuildCommon/BuildLib.cmake)
Include (${common_cmake_dir}/BuildCommon/Install.cmake)

##----------------------------------------------------------------
##
##    テストの設定。
##

Add_SubDirectory (Tests)
