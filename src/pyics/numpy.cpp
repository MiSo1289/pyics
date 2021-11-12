#include <pyics/numpy.hpp>

#include <algorithm>
#include <complex>
#include <cstdint>

#include <pyics/libics_utils.hpp>

namespace pyics
{

auto
to_numpy_type(::Ics_DataType const ics_type) -> pybind11::dtype
{
    switch (ics_type)
    {
        case Ics_uint8:
            return pybind11::dtype::of<std::uint8_t>();
        case Ics_sint8:
            return pybind11::dtype::of<std::int8_t>();
        case Ics_uint16:
            return pybind11::dtype::of<std::uint16_t>();
        case Ics_sint16:
            return pybind11::dtype::of<std::int16_t>();
        case Ics_uint32:
            return pybind11::dtype::of<std::uint32_t>();
        case Ics_sint32:
            return pybind11::dtype::of<std::int32_t>();
        case Ics_uint64:
            return pybind11::dtype::of<std::uint64_t>();
        case Ics_sint64:
            return pybind11::dtype::of<std::int64_t>();
        case Ics_real32:
            return pybind11::dtype::of<float>();
        case Ics_real64:
            return pybind11::dtype::of<double>();
        case Ics_complex32:
            return pybind11::dtype::of<std::complex<float>>();
        case Ics_complex64:
            return pybind11::dtype::of<std::complex<double>>();
        default:
            throw IcsError{ IcsErr_UnknownDataType };
    }
}

auto
to_ics_type(pybind11::dtype const& numpy_type) -> ::Ics_DataType
{
    if (numpy_type.equal(pybind11::dtype::of<std::uint8_t>()))
        return Ics_uint8;
    if (numpy_type.equal(pybind11::dtype::of<std::int8_t>()))
        return Ics_sint8;
    if (numpy_type.equal(pybind11::dtype::of<std::uint16_t>()))
        return Ics_uint16;
    if (numpy_type.equal(pybind11::dtype::of<std::int16_t>()))
        return Ics_sint16;
    if (numpy_type.equal(pybind11::dtype::of<std::uint32_t>()))
        return Ics_uint32;
    if (numpy_type.equal(pybind11::dtype::of<std::int32_t>()))
        return Ics_sint32;
    if (numpy_type.equal(pybind11::dtype::of<std::uint64_t>()))
        return Ics_uint64;
    if (numpy_type.equal(pybind11::dtype::of<std::int64_t>()))
        return Ics_sint64;
    if (numpy_type.equal(pybind11::dtype::of<float>()))
        return Ics_real32;
    if (numpy_type.equal(pybind11::dtype::of<double>()))
        return Ics_real64;
    if (numpy_type.equal(pybind11::dtype::of<std::complex<float>>()))
        return Ics_complex32;
    if (numpy_type.equal(pybind11::dtype::of<std::complex<double>>()))
        return Ics_complex64;

    throw IcsError{ IcsErr_UnknownDataType };
}

auto
read_numpy_array(Ics const& ics) -> pybind11::array
{
    auto const layout = ics.layout();
    auto dimensions = std::vector<std::size_t>{};
    std::ranges::copy(layout.dimensions(), std::back_inserter(dimensions));
    std::ranges::reverse(dimensions);

    auto array =
        pybind11::array{ to_numpy_type(layout.data_type()), dimensions };

    ics.read(std::span{
        static_cast<std::byte*>(array.mutable_data()),
        static_cast<std::size_t>(array.nbytes()),
    });

    return array;
}

void
write_numpy_array(Ics& ics, pybind11::array const& array)
{
    if (not(array.flags() & pybind11::array::c_style))
    {
        throw std::runtime_error{
            "Only C-style (row-major) numpy arrays are supported",
        };
    }

    auto dimensions =
        std::vector<std::size_t>(array.shape(), array.shape() + array.ndim());
    std::ranges::reverse(dimensions);

    ics.set_layout(Layout{ to_ics_type(array.dtype()), dimensions });
    ics.write(std::span{
        static_cast<const std::byte*>(array.data()),
        static_cast<std::size_t>(array.nbytes()),
    });
}

} // namespace pyics
