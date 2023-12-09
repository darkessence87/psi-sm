
#include "TestHelper.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "psi/sm/BaseContext.h"
#include "psi/sm/BaseState.h"

using namespace ::testing;
using namespace psi::sm;

class BaseStateTests : public Test
{
public:
    struct EvTestDiscard {
        friend bool operator==(const EvTestDiscard &, const EvTestDiscard &)
        {
            return true;
        }
    };

    struct EvTestDefer {
        friend bool operator==(const EvTestDefer &, const EvTestDefer &)
        {
            return true;
        }
    };

    struct ITestState {
        virtual ~ITestState() = default;

        virtual const std::string &name() const = 0;

        virtual ProcessResult react(const EvTestDiscard &) = 0;
        virtual ProcessResult react(const EvTestDefer &) = 0;
    };

    struct TestContext : BaseContext<ITestState> {
        const std::deque<Func> &posted()
        {
            return m_posted;
        }
    };

    struct TestState : BaseState<ITestState> {
        TestState()
            : BaseState<ITestState>("TestState")
        {
        }

        ProcessResult react(const EvTestDiscard &) override
        {
            return discard_event();
        }

        ProcessResult react(const EvTestDefer &) override
        {
            return defer_event();
        }

        template <typename T>
        void post_event(const T &ev)
        {
            BaseState<ITestState>::post_event(ev);
        }

        template <typename NextState>
        ProcessResult transit(bool postEvent = false)
        {
            return BaseState<ITestState>::transit<NextState>(postEvent);
        }
    };

    struct AnotherState : BaseState<ITestState> {
        AnotherState()
            : BaseState<ITestState>("AnotherState")
        {
        }

        ProcessResult react(const EvTestDiscard &) override
        {
            return discard_event();
        }

        ProcessResult react(const EvTestDefer &) override
        {
            return defer_event();
        }

        template <typename NextState>
        ProcessResult transit(bool postEvent = false)
        {
            return BaseState<ITestState>::transit<NextState>(postEvent);
        }
    };
};

TEST_F(BaseStateTests, name)
{
    StrictMock<TestContext> context;
    TestState state;
    EXPECT_EQ(state.name(), "TestState");
}

TEST_F(BaseStateTests, transit)
{
    StrictMock<TestContext> context;
    context.transit<TestState>();

    {
        SCOPED_TRACE("// case 1. postEvent = false (default)");

        ASSERT_EQ(context.currentState().has_value(), true);
        EXPECT_EQ(context.currentState().value()->name(), "TestState");

        TestState &state = dynamic_cast<TestState &>(*context.currentState().value());
        EXPECT_EQ(state.transit<AnotherState>(), ProcessResult::TransitState);

        ASSERT_EQ(context.currentState().has_value(), true);
        EXPECT_EQ(context.currentState().value()->name(), "AnotherState");
    }

    {
        SCOPED_TRACE("// case 2. postEvent = true");

        ASSERT_EQ(context.currentState().has_value(), true);
        EXPECT_EQ(context.currentState().value()->name(), "AnotherState");

        AnotherState &state = dynamic_cast<AnotherState &>(*context.currentState().value());
        EXPECT_EQ(state.transit<TestState>(true), ProcessResult::PostedEvent);

        ASSERT_EQ(context.currentState().has_value(), true);
        EXPECT_EQ(context.currentState().value()->name(), "TestState");
    }
}

TEST_F(BaseStateTests, discard_event)
{
    StrictMock<TestContext> context;
    TestState state;
    EXPECT_EQ(state.react(EvTestDiscard {}), ProcessResult::DiscardedEvent);
}

TEST_F(BaseStateTests, defer_event)
{
    StrictMock<TestContext> context;
    TestState state;
    EXPECT_EQ(state.react(EvTestDefer {}), ProcessResult::DeferredEvent);
}

TEST_F(BaseStateTests, post_event)
{
    StrictMock<TestContext> context;
    context.transit<TestState>();
    EXPECT_EQ(context.posted().empty(), true);

    TestState &state = dynamic_cast<TestState &>(*context.currentState().value());
    EvTestDiscard ev;
    state.post_event(ev);

    EXPECT_EQ(context.posted().empty(), false);
}
