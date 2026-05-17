cmake_minimum_required(VERSION 3.24)

project(Booker CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CORE_NAME booker_core)
set(CLI_NAME booker_cli)
set(GUI_NAME booker)

include(FetchContent)
include(ExternalProject)

# Helpers

set(DEPS_INSTALL_DIR "${CMAKE_BINARY_DIR}/deps-install")
file(MAKE_DIRECTORY "${DEPS_INSTALL_DIR}/include")

if(MSVC)
	set(STATIC_LIB_PREFIX "")
	set(STATIC_LIB_SUFFIX ".lib")
else()
	set(STATIC_LIB_PREFIX "lib")
	set(STATIC_LIB_SUFFIX ".a")
endif()

# List sources.

add_subdirectory(core)
add_subdirectory(cli)
add_subdirectory(gui)