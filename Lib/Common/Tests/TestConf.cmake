
##----------------------------------------------------------------
##
##    テストの設定。
##

add_test(NAME   SampleDocumentTest
    COMMAND  $<TARGET_FILE:SampleDocumentTest>
)

add_test(NAME   DebuggerProjectTest
    COMMAND  $<TARGET_FILE:DebuggerProjectTest>
)

##----------------------------------------------------------------
##
##    テストプログラムのビルド。
##

add_executable(SampleDocumentTest       SampleDocumentTest.cpp)
add_executable(DebuggerProjectTest      DebuggerProjectTest.cpp)
