#pragma once

#include <string>

#include "psi/sm/ProcessResult.h"

namespace psi::examples {

struct EvAdd;
struct EvSubstract;

struct ISomeState {
    virtual ~ISomeState() = default;

    virtual const std::string &name() const = 0;

    virtual sm::ProcessResult react(const EvAdd &)
    {
        return sm::ProcessResult::UnconsumedEvent;
    }

    virtual sm::ProcessResult react(const EvSubstract &)
    {
        return sm::ProcessResult::UnconsumedEvent;
    }
};

} // namespace psi::examples