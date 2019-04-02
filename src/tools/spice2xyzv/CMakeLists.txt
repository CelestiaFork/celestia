# allow to build independently of Celestia
if ((NOT ENABLE_SPICE) OR (NOT CSPICE::CSPICE))
  list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake")
  include(FindPackageHandleStandardArgs)
  include(FindCspice)
  find_package(Cspice)
  if(NOT CSPICE_FOUND)
    message(STATUS "Using cspice submodule")
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../../../thirdparty/Spice" "${CMAKE_BINARY_DIR}")

    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../thirdparty/Spice/include")
    add_library(CSPICE::CSPICE ALIAS Spice)
  endif()
endif()

add_executable(spice2xyzv "spice2xyzv.cpp")
target_link_libraries(spice2xyzv CSPICE::CSPICE)
