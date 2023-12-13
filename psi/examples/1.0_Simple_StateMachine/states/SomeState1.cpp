#include "SomeState1.h"

#include "../SomeContext.h"
#include "../SomeExternalClass.h"
#include "../events/SomeEvents.h"
#include "SomeState2.h"

namespace psi::examples {

sm::ProcessResult SomeState1::react(const EvAdd &ev)
{
    auto v = context<SomeContext>()->someExternalClass().getSomeValue();
    v += ev.value;
    context<SomeContext>()->someExternalClass().setSomeValue(v);

    return transit<SomeState2>();
}

sm::ProcessResult SomeState1::react(const EvSubstract &)
{
    return discard_event();
}

} // namespace psi::examples