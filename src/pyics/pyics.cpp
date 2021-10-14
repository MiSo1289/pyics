#include <string>

#include <libics.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <pyics/ics.hpp>
#include <pyics/numpy.hpp>

PYBIND11_MODULE(pyics, mod)
{
    using namespace pybind11::literals;

    mod.def(
        "ics_read",
        [](std::string const& path) -> pybind11::array
        {
            auto ics = pyics::Ics{ path, "r" };
            return pyics::read_numpy_array(ics);
        },
        "path"_a);

    mod.def(
        "ics_write",
        [](std::string const& path,
           pybind11::array const& array,
           std::optional<std::size_t> const gzip_compression_level)
        {
            auto ics = pyics::Ics{ path, "w2" };

            if (gzip_compression_level)
            {
                ics.set_gzip_compression_level(*gzip_compression_level);
            }
            else
            {
                ics.set_uncompressed();
            }

            pyics::write_numpy_array(ics, array);

            // Explicit close() to throw close errors as exceptions
            ics.close();
        },
        "path"_a,
        "array"_a,
        "gzip_compression_level"_a = 6u);
}
