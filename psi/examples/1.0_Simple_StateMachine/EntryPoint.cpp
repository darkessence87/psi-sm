#include "SomeContext.h"
#include "SomeExternalClass.h"
#include "events/SomeEvents.h"

int main()
{
    using namespace psi::examples;

    SomeExternalClass extClass;
    SomeContext context(extClass);

    // 0 + 10 - 20 + 30 - 40 = -20
    context.process_event(EvAdd(10));
    context.process_event(EvSubstract(20));
    context.process_event(EvAdd(30));
    context.process_event(EvAdd(30));
    context.process_event(EvSubstract(40));
    context.process_event(EvSubstract(40));

    LOG_TRACE_STATIC("result: " << extClass.getSomeValue());
}