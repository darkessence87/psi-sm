#pragma once

namespace psi::examples {

struct EvAdd {
    EvAdd(int64_t v)
        : value(v)
    {
    }

    int64_t value;
};

struct EvSubstract {
    EvSubstract(int64_t v)
        : value(v)
    {
    }

    int64_t value;
};

} // namespace psi::examples
