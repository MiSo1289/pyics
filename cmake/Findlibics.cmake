include(FindPackageHandleStandardArgs)

find_path(LIBICS_INCLUDE_DIR libics.h)
find_library(LIBICS_LIBRARY ics)

find_package_handle_standard_args(
  libics

  LIBICS_INCLUDE_DIR
  LIBICS_LIBRARY
)

mark_as_advanced(
  LIBICS_INCLUDE_DIR
  LIBICS_LIBRARY
)

if(LIBICS_FOUND)
  set(LIBICS_INCLUDE_DIRS "${LIBICS_INCLUDE_DIR}")
  set(LIBICS_LIBRARIES "${LIBICS_LIBRARY}")

  add_library(ics::ics INTERFACE IMPORTED)
  set_target_properties(ics::ics PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${LIBICS_INCLUDE_DIRS}")
  set_target_properties(ics::ics PROPERTIES INTERFACE_LINK_LIBRARIES "${LIBICS_LIBRARIES}")
endif()
