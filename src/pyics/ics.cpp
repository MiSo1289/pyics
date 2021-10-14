#include <pyics/ics.hpp>

namespace pyics
{

Ics::Ics(const std::filesystem::path& path, const std::string& mode)
    : _ics{ ics_open(path, mode) }
{
}

auto
Ics::handle() const noexcept -> ::ICS*
{
    return _ics.get();
}

auto
Ics::layout() const -> std::pair<::Ics_DataType, std::vector<std::size_t>>
{
    auto data_type = ::Ics_DataType{};
    auto dimensions = std::vector<std::size_t>(ICS_MAXDIM);
    auto num_dimensions = int{};
    ics_try(::IcsGetLayout(
        handle(), &data_type, &num_dimensions, dimensions.data()));

    dimensions.resize(static_cast<std::size_t>(num_dimensions));

    return { data_type, std::move(dimensions) };
}

auto
Ics::size_bytes() const noexcept -> std::size_t
{
    return ::IcsGetDataSize(handle());
}

void
Ics::read(std::span<std::byte> const buffer) const
{
    ics_try(::IcsGetData(handle(), buffer.data(), buffer.size()));
}

void
Ics::set_layout(::Ics_DataType const data_type,
                std::span<std::size_t const> const dimensions)
{
    ics_try(::IcsSetLayout(handle(),
                           data_type,
                           static_cast<int>(dimensions.size()),
                           dimensions.data()));
}

void
Ics::set_uncompressed()
{
    ics_try(::IcsSetCompression(handle(), IcsCompr_uncompressed, 0));
}

void
Ics::set_gzip_compression_level(std::size_t const level)
{
    ics_try(
        ::IcsSetCompression(handle(), IcsCompr_gzip, static_cast<int>(level)));
}

void
Ics::write(std::span<std::byte const> const buffer)
{
    ics_try(::IcsSetData(handle(), buffer.data(), buffer.size()));
}

void
Ics::close()
{
    ics_try(::IcsClose(_ics.release()));
}

} // namespace pyics
