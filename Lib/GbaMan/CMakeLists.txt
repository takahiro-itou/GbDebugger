
##----------------------------------------------------------------
##
##    モジュールの設定。
##

set(module_title        "GbaMan")

Include (${common_cmake_dir}/BuildCommon/ConfigModule.cmake)
Include (${common_cmake_dir}/BuildCommon/ConfigTarget.cmake)

##----------------------------------------------------------------
##
##    ソースファイルとヘッダファイルのリスト。
##

set(library_source_files
       BaseCpuCore.cpp
       GbaManager.cpp
       MemoryManager.cpp
       CpuArm/ArmALU.cpp
       CpuArm/ArmBranch.cpp
       CpuArm/ArmPsr.cpp
       CpuArm/ArmStrLdr.cpp
       CpuArm/CpuArm.cpp
       CpuArm/DisArm.cpp
       CpuThumb/CpuThumb.cpp
       CpuThumb/DisThumb.cpp
)

set(inclib_header_files
        ${inclib_source_dir}/BaseCpuCore.h
        ${inclib_source_dir}/CpuUtils.h
        ${inclib_source_dir}/GbaManager.h
        ${inclib_source_dir}/MemoryManager.h
        ${inclib_source_dir}/MemoryTable.h
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
