#pragma once

#include <libics.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include <pyics/ics.hpp>

namespace pyics
{

[[nodiscard]] auto to_numpy_type(::Ics_DataType ics_type) -> pybind11::dtype;

[[nodiscard]] auto to_ics_type(pybind11::dtype const& numpy_type)
    -> ::Ics_DataType;

[[nodiscard]] auto read_numpy_array(Ics const& ics) -> pybind11::array;

void write_numpy_array(Ics& ics, pybind11::array const& array);

} // namespace pyics
