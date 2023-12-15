# Description
[**BaseContext**](https://github.com/darkessence87/psi-sm/blob/master/psi/include/psi/sm/BaseContext.h) class is a thread-safe state machine.
The concept is:
- only one state [BaseState](https://github.com/darkessence87/psi-sm/blob/master/psi/include/psi/sm/BaseState.h) exists at any time
- events can be anything fast copyable
- posted events are always processed before deferred events
- deferred events are re-processed only after state is changed
- **IState** is a state's interface with at least one 'ProcessResult react(const T &event)' method:
> virtual ProcessResult react(const EvExample&) { return ProcessResult::UnconsumedEvent; }

# Usage examples
* [1.0 Simple StateMachine](https://github.com/darkessence87/psi-sm/tree/master/psi/examples/1.0_Simple_StateMachine)
