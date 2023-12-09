include_directories(
    include
)

set(TEST_SRC
    tests/BaseContextTests.cpp
    tests/BaseStateTests.cpp
)
psi_make_tests("Context" "${TEST_SRC}" "")

set(EXAMPLE_SRC
    examples/1.0_Simple_StateMachine/EntryPoint.cpp
    examples/1.0_Simple_StateMachine/SomeContext.cpp
    examples/1.0_Simple_StateMachine/states/SomeState1.cpp
    examples/1.0_Simple_StateMachine/states/SomeState2.cpp
)
psi_make_examples("1.0_Simple_StateMachine" "${EXAMPLE_SRC}")