#include "SomeState2.h"

#include "../SomeContext.h"
#include "../SomeExternalClass.h"
#include "../events/SomeEvents.h"
#include "SomeState1.h"

namespace psi::examples {

sm::ProcessResult SomeState2::react(const EvAdd &)
{
    return discard_event();
}

sm::ProcessResult SomeState2::react(const EvSubstract &ev)
{
    auto v = context<SomeContext>()->someExternalClass().getSomeValue();
    v -= ev.value;
    context<SomeContext>()->someExternalClass().setSomeValue(v);

    return transit<SomeState1>();
}

} // namespace psi::examples