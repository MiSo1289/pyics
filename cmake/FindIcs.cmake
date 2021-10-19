include(FindPackageHandleStandardArgs)

find_path(Ics_INCLUDE_DIR libics.h)
find_library(Ics_LIBRARY ics)

file(READ "${Ics_INCLUDE_DIR}/libics.h" LIBICS_H)
string(
  REGEX
  MATCH "#define ICSLIB_VERSION \"([0-9\\.]*)\""
  _
  "${LIBICS_H}"
)
set(Ics_VERSION "${CMAKE_MATCH_1}")

find_package_handle_standard_args(
  Ics
  FOUND_VAR Ics_FOUND
  VERSION_VAR Ics_VERSION
  REQUIRED_VARS
  Ics_LIBRARY
  Ics_INCLUDE_DIR
)

mark_as_advanced(
  Ics_INCLUDE_DIR
  Ics_LIBRARY
)

if(Ics_FOUND AND NOT TARGET Ics::Ics)
  set(Ics_INCLUDE_DIRS "${Ics_INCLUDE_DIR}")
  set(Ics_LIBRARIES "${Ics_LIBRARY}")

  add_library(Ics::Ics UNKNOWN IMPORTED)
  set_target_properties(
    Ics::Ics
    PROPERTIES
    IMPORTED_LOCATION "${Ics_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${Ics_INCLUDE_DIRS}"
  )
endif()
