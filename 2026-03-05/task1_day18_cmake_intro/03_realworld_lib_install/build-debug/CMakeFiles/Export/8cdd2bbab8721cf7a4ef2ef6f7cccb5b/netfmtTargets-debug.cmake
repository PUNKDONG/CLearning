#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "netfmt::net_tool" for configuration "Debug"
set_property(TARGET netfmt::net_tool APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(netfmt::net_tool PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/net_tool"
  )

list(APPEND _cmake_import_check_targets netfmt::net_tool )
list(APPEND _cmake_import_check_files_for_netfmt::net_tool "${_IMPORT_PREFIX}/bin/net_tool" )

# Import target "netfmt::netfmt_shared" for configuration "Debug"
set_property(TARGET netfmt::netfmt_shared APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(netfmt::netfmt_shared PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libnetfmt_shared.so"
  IMPORTED_SONAME_DEBUG "libnetfmt_shared.so"
  )

list(APPEND _cmake_import_check_targets netfmt::netfmt_shared )
list(APPEND _cmake_import_check_files_for_netfmt::netfmt_shared "${_IMPORT_PREFIX}/lib/libnetfmt_shared.so" )

# Import target "netfmt::netfmt_static" for configuration "Debug"
set_property(TARGET netfmt::netfmt_static APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(netfmt::netfmt_static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libnetfmt_static.a"
  )

list(APPEND _cmake_import_check_targets netfmt::netfmt_static )
list(APPEND _cmake_import_check_files_for_netfmt::netfmt_static "${_IMPORT_PREFIX}/lib/libnetfmt_static.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
