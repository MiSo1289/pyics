#pragma once

#include <cstddef>
#include <filesystem>
#include <span>
#include <string>
#include <utility>
#include <vector>

#include <libics.h>

#include <pyics/libics_utils.hpp>

namespace pyics
{

class Ics
{
public:
    explicit Ics(std::filesystem::path const& path, std::string const& mode);

    [[nodiscard]] auto handle() const noexcept -> ::ICS*;

    [[nodiscard]] auto layout() const
        -> std::pair<::Ics_DataType, std::vector<std::size_t>>;

    [[nodiscard]] auto size_bytes() const noexcept -> std::size_t;

    void read(std::span<std::byte> buffer) const;

    void set_layout(::Ics_DataType data_type,
                    std::span<std::size_t const> dimensions);

    void set_uncompressed();

    void set_gzip_compression_level(std::size_t level);

    void write(std::span<std::byte const> buffer);

    void close();

private:
    IcsPtr _ics;
};

} // namespace pyics
