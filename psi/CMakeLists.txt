include_directories(
    ${psi_logger_dir}/psi/include
    include
)

link_directories(
    ${psi_logger_dir}/build/bin/${CMAKE_BUILD_TYPE}
    ${psi_shared_dir}/build/bin/${CMAKE_BUILD_TYPE}
)

set(TEST_SRC
    tests/BaseContextTests.cpp
    tests/BaseStateTests.cpp
)
psi_make_tests("Context" "${TEST_SRC}" "psi-logger;psi-shared")

set(EXAMPLE_SRC
    examples/1.0_Simple_StateMachine/EntryPoint.cpp
    examples/1.0_Simple_StateMachine/SomeContext.cpp
    examples/1.0_Simple_StateMachine/states/SomeState1.cpp
    examples/1.0_Simple_StateMachine/states/SomeState2.cpp
)
psi_make_examples("1.0_Simple_StateMachine" "${EXAMPLE_SRC}" "psi-logger;psi-shared")