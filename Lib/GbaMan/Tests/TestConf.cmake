
##----------------------------------------------------------------
##
##    テストの設定。
##

add_test(NAME   GbaManagerTest
    COMMAND  $<TARGET_FILE:GbaManagerTest>
)

##----------------------------------------------------------------
##
##    テストプログラムのビルド。
##

add_executable(GbaManagerTest           GbaManagerTest.cpp)
