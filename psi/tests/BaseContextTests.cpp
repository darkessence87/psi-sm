
#include "TestHelper.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "psi/sm/BaseContext.h"
#include "psi/sm/BaseState.h"

using namespace ::testing;
using namespace psi::sm;

class BaseContextTests : public Test
{
public:
    struct EvTest {
        friend bool operator==(const EvTest &, const EvTest &)
        {
            return true;
        }
    };

    struct ITestState {
        virtual ~ITestState() = default;

        virtual const std::string &name() const = 0;

        MOCK_METHOD(ProcessResult, react, (const EvTest &), ());
    };

    struct TestContext : BaseContext<ITestState> {
    };

    struct TestState1 : BaseState<ITestState> {
        TestState1()
            : BaseState<ITestState>("TestState1")
        {
        }
    };

    struct TestState2 : BaseState<ITestState> {
        TestState2()
            : BaseState<ITestState>("TestState2")
        {
        }
    };
};

TEST_F(BaseContextTests, process_event)
{
    struct TestContext_PartlyMocked : TestContext {
        MOCK_METHOD(void, process_queue, (), ());

        const std::deque<Func> &deferred()
        {
            return m_deferred;
        }

        const std::deque<Func> &posted()
        {
            return m_posted;
        }
    };
    StrictMock<TestContext_PartlyMocked> context;

    {
        SCOPED_TRACE("// case 1. state not exists");

        EvTest ev;
        EXPECT_EQ(std::nullopt, context.currentState());
        context.process_event(ev);
    }

    {
        SCOPED_TRACE("// case 2. state exists");

        EXPECT_CALL(context, process_queue());
        context.transit<StrictMock<TestState1>>();

        EvTest ev;
        const auto &state = context.currentState();
        ASSERT_EQ(state.has_value(), true);

        {
            SCOPED_TRACE("// case 2.1. react result: DeferredEvent");

            EXPECT_CALL(*state.value(), react(ev)).WillOnce(Return(ProcessResult::DeferredEvent));
            context.process_event(ev);
            EXPECT_EQ(context.deferred().size(), 1);

            EXPECT_CALL(*state.value(), react(ev)).WillOnce(Return(ProcessResult::UnconsumedEvent));
            context.deferred().front()();
        }

        {
            SCOPED_TRACE("// case 2.2. react result: TransitState");

            EXPECT_CALL(*state.value(), react(ev)).WillOnce(Return(ProcessResult::TransitState));
            EXPECT_CALL(context, process_queue());
            context.process_event(ev);
        }

        {
            SCOPED_TRACE("// case 2.3. react result: PostedEvent");

            EXPECT_CALL(*state.value(), react(ev)).WillOnce(Return(ProcessResult::PostedEvent));
            EXPECT_CALL(context, process_queue());
            context.process_event(ev);
            EXPECT_EQ(context.posted().size(), 1);

            EXPECT_CALL(*state.value(), react(ev)).WillOnce(Return(ProcessResult::UnconsumedEvent));
            context.posted().front()();
        }

        {
            SCOPED_TRACE("// case 2.4. react result: UnconsumedEvent");

            EXPECT_CALL(*state.value(), react(ev)).WillOnce(Return(ProcessResult::UnconsumedEvent));
            context.process_event(ev);
        }

        {
            SCOPED_TRACE("// case 2.5. react result: DiscardedEvent");

            EXPECT_CALL(*state.value(), react(ev)).WillOnce(Return(ProcessResult::DiscardedEvent));
            context.process_event(ev);
        }
    }
}

TEST_F(BaseContextTests, post_event)
{
    struct TestContext_PartlyMocked : TestContext {
        MOCK_METHOD(void, process_queue, (), ());

        const std::deque<Func> &posted()
        {
            return m_posted;
        }
    };
    StrictMock<TestContext_PartlyMocked> context;

    EXPECT_CALL(context, process_queue());
    context.transit<StrictMock<TestState1>>();
    const auto &state = context.currentState();
    ASSERT_EQ(state.has_value(), true);

    EvTest ev;
    context.post_event(ev);

    EXPECT_CALL(*state.value(), react(ev)).WillOnce(Return(ProcessResult::UnconsumedEvent));
    context.posted().front()();
}

TEST_F(BaseContextTests, transit)
{
    struct TestContext_PartlyMocked : TestContext {
        MOCK_METHOD(void, process_queue, (), ());
    };
    StrictMock<TestContext_PartlyMocked> context;

    {
        SCOPED_TRACE("// case 1. no initial state");

        const auto &oldState = context.currentState();
        ASSERT_EQ(oldState.has_value(), false);

        EXPECT_CALL(context, process_queue());
        context.transit<TestState1>();

        const auto &newState = context.currentState();
        ASSERT_EQ(newState.has_value(), true);
        EXPECT_EQ(newState.value()->name(), "TestState1");
    }

    {
        SCOPED_TRACE("// case 2. transit old -> new state");

        const auto &oldState = context.currentState();
        ASSERT_EQ(oldState.has_value(), true);
        EXPECT_EQ(oldState.value()->name(), "TestState1");

        EXPECT_CALL(context, process_queue());
        context.transit<TestState2>();

        const auto &newState = context.currentState();
        ASSERT_EQ(newState.has_value(), true);
        EXPECT_EQ(newState.value()->name(), "TestState2");
    }
}

TEST_F(BaseContextTests, process_queue)
{
    struct TestContext_PartlyMocked : TestContext {
        void process_queue()
        {
            TestContext::process_queue();
        }
    };
    StrictMock<TestContext_PartlyMocked> context;

    {
        SCOPED_TRACE("// case 1. queue is empty");

        context.process_queue();
    }

    {
        SCOPED_TRACE("// case 2. transitionFound = false");

        // context.process_queue();
    }
}
// virtual void process_queue()
//     {
//         m_queue.insert(m_queue.begin(), m_posted.begin(), m_posted.end());
//         m_posted.clear();

//         m_queue.insert(m_queue.begin(), m_deferred.begin(), m_deferred.end());
//         m_deferred.clear();

//         if (m_queue.empty()) {
//             return;
//         }

//         bool transitionFound = false;
//         while (!m_queue.empty()) {
//             auto fn = m_queue.front();
//             m_queue.pop_front();

//             auto rs = fn();

//             switch (rs) {
//             case ProcessResult::DeferredEvent:
//                 m_deferred.emplace_back(fn);
//                 LOG_TRACE("[" << m_state->name() << "] re-defer event. Queue size: " << queueSize());
//                 break;

//             case ProcessResult::TransitState:
//                 transitionFound = true;
//                 break;

//             case ProcessResult::PostedEvent:
//                 m_posted.emplace_back(fn);
//                 LOG_TRACE("[" << m_state->name() << "] post event. Queue size: " << queueSize());
//                 transitionFound = true;
//                 break;

//             case ProcessResult::DiscardedEvent:
//                 LOG_TRACE("[" << m_state->name() << "] discard event. Queue size: " << queueSize());
//                 break;
//             }

//             if (transitionFound) {
//                 break;
//             }
//         }

//         if (transitionFound) {
//             process_queue();
//         }
//     }