
##----------------------------------------------------------------
##
##    テストの設定。
##

add_test(NAME   DebuggerProjectTest
    COMMAND  $<TARGET_FILE:DebuggerProjectTest>
)

add_test(NAME   DebuggerTypesTest
    COMMAND  $<TARGET_FILE:DebuggerTypesTest>
)

##----------------------------------------------------------------
##
##    テストプログラムのビルド。
##

add_executable(DebuggerProjectTest      DebuggerProjectTest.cpp)
add_executable(DebuggerTypesTest        DebuggerTypesTest.cpp)
