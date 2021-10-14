#pragma once

#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>
#include <system_error>

#include <libics.h>

namespace pyics
{

[[nodiscard]] auto ics_error_category() -> std::error_category const&;

class IcsError : public std::system_error
{
public:
    [[nodiscard]] explicit IcsError(::Ics_Error const error)
        : system_error{ static_cast<int>(error), ics_error_category() }
    {
    }
};

void ics_try(::Ics_Error error);

class IcsDeleter
{
public:
    using pointer = ::ICS*;

    void operator()(pointer const ptr) const noexcept { ::IcsClose(ptr); }
};

using IcsPtr = std::unique_ptr<::ICS, IcsDeleter>;

[[nodiscard]] auto ics_open(std::filesystem::path const& path,
                            std::string const& mode) -> IcsPtr;

} // namespace pyics
