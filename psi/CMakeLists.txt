set(target_lib "psi-sm")

add_library(${target_lib} INTERFACE)
add_library(psi::sm ALIAS ${target_lib})

target_include_directories(${target_lib}
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_compile_features(${target_lib} INTERFACE cxx_std_20)

set(TEST_SRC
    tests/BaseContextTests.cpp
    tests/BaseStateTests.cpp
)
# psi_make_tests("Context" "${TEST_SRC}" "${target_lib}")

set(EXAMPLE_SRC
    examples/1.0_Simple_StateMachine/EntryPoint.cpp
    examples/1.0_Simple_StateMachine/SomeContext.cpp
    examples/1.0_Simple_StateMachine/states/SomeState1.cpp
    examples/1.0_Simple_StateMachine/states/SomeState2.cpp
)
psi_make_examples("1.0_Simple_StateMachine" "${EXAMPLE_SRC}" "${target_lib}")