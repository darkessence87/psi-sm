#pragma once

#include "ISomeState.h"

#include "psi/sm/BaseContext.h"

namespace psi::examples {

class SomeExternalClass;

class SomeContext : public sm::BaseContext<ISomeState>
{
public:
    SomeContext(SomeExternalClass &);

    SomeExternalClass &someExternalClass();

private:
    SomeExternalClass &m_someExternalClass;
};

} // namespace psi::examples