
##----------------------------------------------------------------
##
##    テストの設定。
##

add_test(NAME   GbaManagerTest
    COMMAND  $<TARGET_FILE:GbaManagerTest>
)

add_test(NAME   MemoryManagerTest
    COMMAND  $<TARGET_FILE:MemoryManagerTest>
)

##----------------------------------------------------------------
##
##    テストプログラムのビルド。
##

add_executable(GbaManagerTest           GbaManagerTest.cpp)
add_executable(MemoryManagerTest        MemoryManagerTest.cpp)
