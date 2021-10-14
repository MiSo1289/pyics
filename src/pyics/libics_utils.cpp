#include <pyics/libics_utils.hpp>

namespace pyics
{

class IcsErrorCategory : public std::error_category
{
public:
    [[nodiscard]] auto name() const noexcept -> char const* override
    {
        return "ICS";
    }

    [[nodiscard]] auto message(int const code) const -> std::string override
    {
        return ::IcsGetErrorText(static_cast<Ics_Error>(code));
    }
};

auto
ics_error_category() -> std::error_category const&
{
    static constinit auto const category = IcsErrorCategory{};
    return category;
}

void
ics_try(::Ics_Error const error)
{
    if (error != IcsErr_Ok)
    {
        throw IcsError{ error };
    }
}

auto
ics_open(std::filesystem::path const& path, std::string const& mode) -> IcsPtr
{
    auto ics_ptr = static_cast<::ICS*>(nullptr);
    ics_try(::IcsOpen(&ics_ptr, path.c_str(), mode.c_str()));

    return IcsPtr{ ics_ptr };
}

} // namespace pyics
