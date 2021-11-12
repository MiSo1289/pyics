#pragma once

#include <array>
#include <cstddef>
#include <filesystem>
#include <initializer_list>
#include <span>
#include <string>
#include <utility>
#include <vector>

#include <libics.h>

#include <pyics/libics_utils.hpp>

namespace pyics
{

class Layout
{
public:
    static constexpr auto max_dimensions = std::size_t{ ICS_MAXDIM };

    [[nodiscard]] Layout() noexcept = default;

    [[nodiscard]] Layout(::Ics_DataType const data_type,
                         std::initializer_list<std::size_t> const dimensions)
        : Layout{ data_type, std::span{ dimensions } }
    {
    }

    [[nodiscard]] Layout(::Ics_DataType data_type,
                         std::span<std::size_t const> dimensions);

    [[nodiscard]] auto data_type() const noexcept -> ::Ics_DataType
    {
        return data_type_;
    }

    [[nodiscard]] auto dimensions() const noexcept
        -> std::span<std::size_t const>
    {
        return std::span{ dimensions_ }.first(num_dimensions_);
    }

private:
    ::Ics_DataType data_type_ = {};
    std::array<std::size_t, max_dimensions> dimensions_ = {};
    std::size_t num_dimensions_ = {};
};

class Ics
{
public:
    explicit Ics(std::filesystem::path const& path, std::string const& mode);

    [[nodiscard]] auto handle() const noexcept -> ::ICS*;

    [[nodiscard]] auto layout() const -> Layout;

    [[nodiscard]] auto size_bytes() const noexcept -> std::size_t;

    void read(std::span<std::byte> buffer) const;

    void set_layout(Layout const& layout);

    void set_uncompressed();

    void set_gzip_compression_level(std::size_t level);

    void write(std::span<std::byte const> buffer);

    void close();

private:
    IcsPtr _ics;
};

} // namespace pyics
