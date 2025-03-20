
##----------------------------------------------------------------
##
##    テストの設定。
##

add_test(NAME   SampleDocumentTest
    COMMAND  $<TARGET_FILE:SampleDocumentTest>
)

add_test(NAME   DebuggerSettingsTest
    COMMAND  $<TARGET_FILE:DebuggerSettingsTest>
)

##----------------------------------------------------------------
##
##    テストプログラムのビルド。
##

add_executable(SampleDocumentTest       SampleDocumentTest.cpp)
add_executable(DebuggerSettingsTest     DebuggerSettingsTest.cpp)
