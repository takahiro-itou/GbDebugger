
##
##    List of Tests.
##

extra_tests             =
TESTS                   =  \
        CpuUtilsTest            \
        GbaManagerTest          \
        MemoryManagerTest       \
        ${extra_tests}

##
##    Test Configurations.
##

target_testee_library       +=  -lgbdGbaMan

dist_noinst_data_files      +=  badrom.gba
dist_noinst_data_files      +=  hello.gba

dist_noinst_header_files    +=
extra_test_drivers          +=
library_test_drivers        +=
source_test_drivers         +=

##
##    Compile and Link Options.
##

test_cppflags_common        +=
test_ldflags_common         +=

##
##    Test Programs.
##

CpuUtilsTest_SOURCES            =  CpuUtilsTest.cpp
GbaManagerTest_SOURCES          =  GbaManagerTest.cpp
MemoryManagerTest_SOURCES       =  MemoryManagerTest.cpp

