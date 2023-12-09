#include "../ISomeState.h"

#include "psi/sm/BaseState.h"

namespace psi::examples {

struct SomeState1 final : public sm::BaseState<ISomeState> {
    SomeState1()
        : BaseState<ISomeState>("SomeState1")
    {
    }

    sm::ProcessResult react(const EvAdd &) override;
    sm::ProcessResult react(const EvSubstract &) override;
};

} // namespace psi::examples