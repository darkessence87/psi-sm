#include "SomeContext.h"

#include "SomeExternalClass.h"

#include "states/SomeState1.h"

namespace psi::examples {

SomeContext::SomeContext(SomeExternalClass &extClass)
    : m_someExternalClass(extClass)
{
    transit<SomeState1>(); // default state
}

SomeExternalClass &SomeContext::someExternalClass()
{
    return m_someExternalClass;
}

} // namespace psi::examples