# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.31)
option(DEVELOPER_MODE "Build tests, warnings as errors" ON)
if(DEVELOPER_MODE)
  list(APPEND VCPKG_MANIFEST_FEATURES "tests")
endif()

project (zstd_stream VERSION 0.0.1 LANGUAGES CXX)

# Include sub-projects.
add_subdirectory ("memlz")
if (DEVELOPER_MODE)
	add_subdirectory(test)
endif()
