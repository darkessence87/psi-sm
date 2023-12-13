#pragma once

namespace psi::examples {

class SomeExternalClass
{
public:
    void setSomeValue(int64_t v)
    {
        m_someValue = v;
    }

    int64_t getSomeValue() const
    {
        return m_someValue;
    }

private:
    int64_t m_someValue = 0;
};

} // namespace psi::examples