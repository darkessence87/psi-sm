#include "../ISomeState.h"

#include "psi/sm/BaseState.h"

namespace psi::examples {

struct SomeState2 final : public sm::BaseState<ISomeState> {
    SomeState2()
        : BaseState<ISomeState>("SomeState2")
    {
    }

    sm::ProcessResult react(const EvAdd &) override;
    sm::ProcessResult react(const EvSubstract &) override;
};

} // namespace psi::examples