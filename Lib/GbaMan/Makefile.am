
##
##    ライブラリのビルドとインストール。
##

AUTOMAKE_OPTIONS    =  subdir-objects

##
##    List of Directories.
##

module_dir_name     =  GbaMan
SUBDIRS             =  .  Tests

include  $(top_srcdir)/.M4/BuildCommon/DirList.inc
include  $(top_srcdir)/.M4/BuildCommon/DefVars.inc

##
##    List of Source and Header Files.
##

extra_library_source_files  =
extra_library_header_files  =
extra_noinst_data_files     =
extra_noinst_header_files   =

library_source_files        +=  \
        BaseCpuCore.cpp                \
        BaseDisCpu.cpp                 \
        GbaManager.cpp                 \
        MemoryManager.cpp              \
        CpuArm/Branch.cpp              \
        CpuArm/ArmPsr.cpp              \
        CpuArm/CpuArm.cpp              \
        CpuArm/DisArm.cpp              \
        CpuArm/StoreLoad.cpp           \
        CpuThumb/CpuThumb.cpp          \
        CpuThumb/DisThumb.cpp          \
        ${extra_library_source_files}

library_instantiations      +=
library_inline_files        +=

dist_noinst_data_files      +=  \
        ${extra_noinst_data_files}

dist_noinst_header_files    +=  \
        CpuArm/ArithmeticLogic.inl     \
        CpuArm/ArmALU.h                \
        CpuArm/InstTable.h             \
        CpuArm/CpuArm.h                \
        CpuArm/DisArm.h                \
        CpuArm/OperatorConst.h         \
        CpuArm/StoreLoad.inl           \
        CpuThumb/Branch.inl            \
        CpuThumb/CpuThumb.h            \
        CpuThumb/DisThumb.h            \
        CpuThumb/OperateRegister.inl   \
        CpuThumb/StoreLoad.inl         \
        CpuThumb/InstTable.h           \
        Utils/DisUtils.inl             \
        Utils/ShiftOperator.h          \
        ${extra_noinst_header_files}

inclib_header_files         +=  \
        ${inclib_source_dir}/BaseCpuCore.h         \
        ${inclib_source_dir}/BaseDisCpu.h          \
        ${inclib_source_dir}/CpuUtils.h            \
        ${inclib_source_dir}/GbaManager.h          \
        ${inclib_source_dir}/MemoryManager.h       \
        ${inclib_source_dir}/MemoryTable.h         \
        ${extra_library_header_files}

inclib_nodist_header_files  +=

library_install_srcfiles    +=  \
        ${library_inline_files}

library_compile_srcfiles    +=  \
        ${library_source_files}    \
        ${library_instantiations}

##
##    List of Libraries.
##

static_libraries_list   +=  libgbdGbaMan.static.a
shared_libraries_list   +=

##
##    Internal Libraries.
##

##
##    Include Common Install Settings.
##

include  $(top_srcdir)/.M4/BuildCommon/Install.inc

##
##    Build Libraries.
##

libgbdGbaMan_static_a_SOURCES   =  ${library_source_files}

