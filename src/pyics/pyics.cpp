#include <algorithm>
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
        "read_ics",
        [](std::string const& path) -> pybind11::array
        {
            auto ics = pyics::Ics{ path, "r" };
            return pyics::read_numpy_array(ics);
        },
        "path"_a);

    mod.def(
        "write_ics",
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

    pybind11::class_<pyics::Ics>{ mod, "Ics" }
        .def("__enter__", [](const pyics::Ics& ics) { return &ics; })
        .def(
            "__exit__",
            [](pyics::Ics& ics,
               pybind11::object const&,
               pybind11::object const&,
               pybind11::object const&) { ics.close(); },
            "exc_type"_a,
            "exc_val"_a,
            "exc_tb"_a)
        .def("layout",
             [](pyics::Ics& ics)
             {
                 auto const layout = ics.layout();
                 auto dimensions = std::vector<std::size_t>{};
                 std::ranges::copy(layout.dimensions(),
                                   std::back_inserter(dimensions));
                 std::ranges::reverse(dimensions);

                 return std::pair{
                     pyics::to_numpy_type(layout.data_type()),
                     std::move(dimensions),
                 };
             })
        .def("size_bytes", &pyics::Ics::size_bytes)
        .def("read",
             [](pyics::Ics& ics) { return pyics::read_numpy_array(ics); })
        .def("set_uncompressed", &pyics::Ics::set_uncompressed)
        .def("set_gzip_compression_level",
             &pyics::Ics::set_gzip_compression_level,
             "level"_a)
        .def(
            "write",
            [](pyics::Ics& ics, pybind11::array const& array)
            { return pyics::write_numpy_array(ics, array); },
            "array"_a)
        .def("close", &pyics::Ics::close);

    mod.def(
        "open_ics",
        [](std::string const& path, std::string const& mode) {
            return pyics::Ics{ path, mode };
        },
        "path"_a,
        "mode"_a);
}
