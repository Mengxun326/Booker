cmake_minimum_required(VERSION 3.24)

project(PDF CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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


# zlib
message(STATUS "Downloading zlib...")

FetchContent_Declare(
	zlib
	GIT_REPOSITORY https://github.com/madler/zlib.git
	GIT_TAG v1.3.1
)

set(ZLIB_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(ZLIB_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(ZLIB_BUILD_TESTS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(zlib)

set_target_properties(zlibstatic PROPERTIES EXPORT_NAME "")

if(TARGET zlib)
	set_target_properties(zlib PROPERTIES EXCLUDE_FROM_ALL TRUE)
endif()

file(WRITE "${CMAKE_BINARY_DIR}/redirects/ZLIBConfig.cmake" "
	if(NOT TARGET ZLIB::ZLIB)
		add_library(ZLIB::ZLIB ALIAS zlibstatic)
	endif()
	
	set(ZLIB_FOUND TRUE)
	set(ZLIB_INCLUDE_DIRS \${zlib_SOURCE_DIR} \${zlib_BINARY_DIR})
	set(ZLIB_LIBRARIES ZLIB::ZLIB)
	set(ZLIB_VERSION_STRING \"1.3.1\")
")

set(ZLIB_LIBRARY zlibstatic CACHE STRING "" FORCE)
set(ZLIB_INCLUDE_DIR "${zlib_SOURCE_DIR};${zlib_BINARY_DIR}" CACHE STRING "" FORCE)

if(NOT TARGET ZLIB::ZLIB)
	add_library(ZLIB::ZLIB ALIAS zlibstatic)
endif()

message(STATUS "zlib downloaded.")

# libjpeg-turbo
message(STATUS "Configuring libjpeg-turbo (ExternalProject)...")

if(MSVC)
	set(JPEG_LIB_FILE "jpeg-static.lib")
else()
	set(JPEG_LIB_FILE "${STATIC_LIB_PREFIX}jpeg${STATIC_LIB_SUFFIX}")
endif()

set(_ep_cmake_args
	-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
	-DCMAKE_INSTALL_PREFIX=${DEPS_INSTALL_DIR}
	-DENABLE_SHARED=OFF
	-DENABLE_STATIC=ON
	-DWITH_TURBOJPEG=ON
)

if(MSVC)
	list(APPEND _ep_cmake_args
				-DCMAKE_GENERATOR=${CMAKE_GENERATOR}
				-DCMAKE_GENERATOR_PLATFORM=${CMAKE_GENERATOR_PLATFORM}
	)
endif()

message(STATUS "Downloading libjpeg-turbo...")

ExternalProject_Add(
	libjpeg-turbo-ext
	GIT_REPOSITORY https://github.com/libjpeg-turbo/libjpeg-turbo.git
	GIT_TAG 3.0.2
	GIT_SHALLOW TRUE
	CMAKE_ARGS ${_ep_cmake_args}
	INSTALL_COMMAND ${CMAKE_COMMAND} --install <BINARY_DIR> --config ${CMAKE_BUILD_TYPE}
	BUILD_BYPRODUCTS "${DEPS_INSTALL_DIR}/lib/${JPEG_LIB_FILE}"
)

message(STATUS "libjpeg-turbo downloaded.")

if(MSVC)
	set(JPEG_LIB_FILE "jpeg-static.lib")
else()
	set(JPEG_LIB_FILE "${STATIC_LIB_PREFIX}jpeg${STATIC_LIB_SUFFIX}")
endif()

add_library(JPEG::JPEG STATIC IMPORTED GLOBAL)
add_dependencies(JPEG::JPEG libjpeg-turbo-ext)

set_target_properties(JPEG::JPEG PROPERTIES
	IMPORTED_LOCATION "${DEPS_INSTALL_DIR}/lib/${JPEG_LIB_FILE}"
	IMPORTED_LOCATION_DEBUG "${DEPS_INSTALL_DIR}/lib/${JPEG_LIB_FILE}"
	IMPORTED_LOCATION_RELEASE "${DEPS_INSTALL_DIR}/lib/${JPEG_LIB_FILE}"
	IMPORTED_LOCATION_MINSIZEREL "${DEPS_INSTALL_DIR}/lib/${JPEG_LIB_FILE}"
	IMPORTED_LOCATION_RELWITHDEBINFO "${DEPS_INSTALL_DIR}/lib/${JPEG_LIB_FILE}"
	INTERFACE_INCLUDE_DIRECTORIES "${DEPS_INSTALL_DIR}/include"
)

file(WRITE "${CMAKE_BINARY_DIR}/redirects/JPEGConfig.cmake" "
	if(NOT TARGET JPEG::JPEG)
		add_library(JPEG::JPEG STATIC IMPORTED GLOBAL)
		set_target_properties(JPEG::JPEG PROPERTIES
			IMPORTED_LOCATION \"${DEPS_INSTALL_DIR}/lib/${JPEG_LIB_FILE}\"
			INTERFACE_INCLUDE_DIRECTORIES \"${DEPS_INSTALL_DIR}/include\"
		)
	endif()
	
	set(JPEG_FOUND TRUE)
	set(JPEG_INCLUDE_DIRS \"${DEPS_INSTALL_DIR}/include\")
	set(JPEG_LIBRARIES JPEG::JPEG)
")

message(STATUS "libjpeg-turbo configured (will build with the project).")

# FreeType
message(STATUS "Downloading FreeType...")

FetchContent_Declare(
	freetype
	GIT_REPOSITORY https://gitlab.freedesktop.org/freetype/freetype.git
	GIT_TAG VER-2-13-3
)

set(FT_DISABLE_ZLIB OFF CACHE BOOL "" FORCE)
set(FT_DISABLE_PNG ON CACHE BOOL "" FORCE)
set(FT_DISABLE_HARFBUZZ ON CACHE BOOL "" FORCE)
set(FT_DISABLE_BZIP2 ON CACHE BOOL "" FORCE)

set(SKIP_INSTALL_ALL ON CACHE BOOL "" FORCE)
set(CMAKE_POLICY_VERSION_MINIMUM "3.5" CACHE STRING "" FORCE)

FetchContent_MakeAvailable(freetype)

file(WRITE "${CMAKE_BINARY_DIR}/redirects/FreetypeConfig.cmake" "
	if(NOT TARGET Freetype::Freetype)
		add_library(Freetype::Freetype ALIAS freetype)
	endif()
	
	set(FREETYPE_FOUND TRUE)
	set(FREETYPE_INCLUDE_DIRS \${freetype_SOURCE_DIR}/include)
	set(FREETYPE_LIBRARIES Freetype::Freetype)
	set(FREETYPE_VERSION_STRING \"2.13.3\")
")

if(NOT TARGET Freetype::Freetype)
	add_library(Freetype::Freetype ALIAS freetype)
endif()

message(STATUS "FreeType downloaded.")

# libpng
message(STATUS "Downloading libpng...")

FetchContent_Declare(
	libpng
	GIT_REPOSITORY https://github.com/pnggroup/libpng.git
	GIT_TAG v1.6.43
)

set(PNG_SHARED OFF CACHE BOOL "" FORCE)
set(PNG_STATIC ON CACHE BOOL "" FORCE)
set(PNG_TESTS OFF CACHE BOOL "" FORCE)
set(PNG_NO_STDIO ON CACHE BOOL "" FORCE)
set(SKIP_INSTALL_ALL ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(libpng)

file(WRITE "${CMAKE_BINARY_DIR}/redirects/PNGConfig.cmake" "
	if(NOT TARGET PNG::PNG)
		add_library(PNG::PNG ALIAS png_static)
	endif()
	
	set(PNG_FOUND TRUE)
	set(PNG_INCLUDE_DIRS \${libpng_SOURCE_DIR} \${libpng_BINARY_DIR})
	set(PNG_LIBRARIES PNG::PNG)
")

if(NOT TARGET PNG::PNG)
	add_library(PNG::PNG ALIAS png_static)
endif()

message(STATUS "libpng downloaded.")

# Iconv
if(WIN32)
	message(STATUS "Downloading and configuring win-iconv...")
	
	if(NOT TARGET iconv_static)
		set(WIN_ICONV_BUILD_EXECUTABLE OFF CACHE BOOL "" FORCE)
		set(WIN_ICONV_BUILD_TEST OFF CACHE BOOL "" FORCE)
		set(WIN_ICONV_BUILD_SHARED OFF CACHE BOOL "" FORCE)
		set(WIN_ICONV_BUILD_STATIC ON CACHE BOOL "" FORCE)
		
		FetchContent_Declare(
			win_iconv
			GIT_REPOSITORY https://github.com/win-iconv/win-iconv.git
			GIT_TAG v0.0.10
			GIT_SHALLOW TRUE
		)
		
		FetchContent_MakeAvailable(win_iconv)
	endif()
	
	if(NOT TARGET Iconv::Iconv)
		add_library(Iconv::Iconv ALIAS iconv_static)
	endif()
	
	set(Iconv_INCLUDE_DIR "${win_iconv_SOURCE_DIR}" CACHE STRING "" FORCE)
	set(Iconv_LIBRARY "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${STATIC_LIB_PREFIX}iconv_static${STATIC_LIB_SUFFIX}" CACHE FILEPATH "" FORCE)
	
	message(STATUS "win-iconv downloaded and configured.")
else()
	if(NOT TARGET Iconv::Iconv)
		add_library(Iconv::Iconv INTERFACE IMPORTED GLOBAL)
	endif()
	
	set(Iconv_FOUND TRUE CACHE BOOL "" FORCE)
	message(STATUS "Iconv::Iconv → built-in libc")
endif()

# Redirects injection before Poppler
set(CMAKE_FIND_PACKAGE_REDIRECTS_DIR "${CMAKE_BINARY_DIR}/redirects")

message(STATUS "Configuring Poppler (ExternalProject)...")

set(POPPLER_INSTALL_DIR "${CMAKE_BINARY_DIR}/poppler-install")

# ZLIB
set(_zlib_lib "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${STATIC_LIB_PREFIX}zlibstatic${STATIC_LIB_SUFFIX}")
set(ZLIB_INCLUDE_DIR "${zlib_SOURCE_DIR};${zlib_BINARY_DIR}")

# Freetype
set(_ft_lib "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${STATIC_LIB_PREFIX}freetype${STATIC_LIB_SUFFIX}")
set(_ft_inc "${freetype_SOURCE_DIR}/include;${freetype_BINARY_DIR}/include")

# JPEG (libjpeg-turbo)
set(_jpeg_lib "${DEPS_INSTALL_DIR}/lib/${JPEG_LIB_FILE}")
set(_jpeg_inc "${DEPS_INSTALL_DIR}/include")

# PNG
set(_png_lib "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${STATIC_LIB_PREFIX}png_static${STATIC_LIB_SUFFIX}")
set(_png_inc "${libpng_SOURCE_DIR};${libpng_BINARY_DIR}")

# Iconv (Windows specific)
if(WIN32)
	set(_iconv_inc "${Iconv_INCLUDE_DIR}")
	set(_iconv_lib "${Iconv_LIBRARY}")
	set(_iconv_extra_args
		-DIconv_INCLUDE_DIR=${_iconv_inc}
		-DIconv_LIBRARY=${_iconv_lib}
		-DIconv_FOUND=TRUE
	)
else()
	set(_iconv_extra_args "")
endif()

# Initial cache file for Poppler
set(POPPLER_INIT_CACHE "${CMAKE_BINARY_DIR}/poppler-init.cmake")

file(WRITE ${POPPLER_INIT_CACHE}
	"set(ZLIB_INCLUDE_DIR \"${zlib_SOURCE_DIR}\;${zlib_BINARY_DIR}\" CACHE PATH \"\")\n"
	"set(PNG_PNG_INCLUDE_DIR \"${libpng_SOURCE_DIR}\;${libpng_BINARY_DIR}\" CACHE PATH \"\")\n"
	"set(FREETYPE_INCLUDE_DIRS \"${freetype_SOURCE_DIR}/include\;${freetype_BINARY_DIR}/include\" CACHE PATH \"\")\n"
)

if(WIN32)
	file(APPEND ${POPPLER_INIT_CACHE}
		"set(Iconv_INCLUDE_DIR \"${_iconv_inc}\" CACHE PATH \"\")\n"
		"set(Iconv_LIBRARY \"${_iconv_lib}\" CACHE FILEPATH \"\")\n"
		"set(ICONV_SECOND_ARGUMENT_IS_CONST TRUE CACHE BOOL \"\")\n"
	)
endif()

set(_poppler_cmake_args
	-C ${POPPLER_INIT_CACHE}
	-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
	-DCMAKE_INSTALL_PREFIX=${POPPLER_INSTALL_DIR}
	-DBUILD_SHARED_LIBS=OFF
	-DFREETYPE_LIBRARY=${_ft_lib}
	-DZLIB_LIBRARY=${_zlib_lib}
	-DJPEG_LIBRARY=${_jpeg_lib}
	-DJPEG_INCLUDE_DIR=${_jpeg_inc}
	-DPNG_LIBRARY=${_png_lib}
	${_iconv_extra_args}
	-DENABLE_CPP=ON
	-DFONT_CONFIGURATION=generic
	-DENABLE_BOOST=OFF
	-DENABLE_QT5=OFF
	-DENABLE_QT6=OFF
	-DENABLE_GLIB=OFF
	-DENABLE_UTILS=OFF
	-DENABLE_LIBCURL=OFF
	-DENABLE_LIBTIFF=OFF
	-DENABLE_LCMS=OFF
	-DENABLE_NSS3=OFF
	-DENABLE_GPGME=OFF
	-DENABLE_LIBOPENJPEG=none
	-DENABLE_DCTDECODER=libjpeg
	-DENABLE_ZLIB_UNCOMPRESS=OFF
	-DENABLE_RELOCATABLE=OFF
	-DBUILD_GTK_TESTS=OFF
	-DBUILD_QT5_TESTS=OFF
	-DBUILD_QT6_TESTS=OFF
	-DBUILD_CPP_TESTS=OFF
	-DBUILD_MANUAL_TESTS=OFF
)

if(MSVC)
	list(APPEND _poppler_cmake_args
		-DCMAKE_GENERATOR=${CMAKE_GENERATOR}
		-DCMAKE_GENERATOR_PLATFORM=${CMAKE_GENERATOR_PLATFORM}
	)
endif()

message(STATUS "Downloading Poppler...")

ExternalProject_Add(
	poppler-ext
	GIT_REPOSITORY https://gitlab.freedesktop.org/poppler/poppler.git
	GIT_TAG poppler-24.03.0
	GIT_SHALLOW TRUE
	CMAKE_ARGS ${_poppler_cmake_args}
	INSTALL_COMMAND ${CMAKE_COMMAND} --install <BINARY_DIR> --config ${CMAKE_BUILD_TYPE}
	BUILD_BYPRODUCTS
		"${POPPLER_INSTALL_DIR}/lib/${STATIC_LIB_PREFIX}poppler${STATIC_LIB_SUFFIX}"
		"${POPPLER_INSTALL_DIR}/lib/${STATIC_LIB_PREFIX}poppler-cpp${STATIC_LIB_SUFFIX}"
)

ExternalProject_Add_StepDependencies(poppler-ext configure libjpeg-turbo-ext zlibstatic freetype png_static iconv_static)

message(STATUS "Poppler configured (will build with the project).")

file(MAKE_DIRECTORY "${POPPLER_INSTALL_DIR}/include")

add_library(poppler STATIC IMPORTED GLOBAL)
add_dependencies(poppler poppler-ext)

set_target_properties(poppler PROPERTIES
	IMPORTED_LOCATION "${POPPLER_INSTALL_DIR}/lib/${STATIC_LIB_PREFIX}poppler${STATIC_LIB_SUFFIX}"
	INTERFACE_INCLUDE_DIRECTORIES "${POPPLER_INSTALL_DIR}/include"
	INTERFACE_LINK_LIBRARIES "Freetype::Freetype;ZLIB::ZLIB;JPEG::JPEG;PNG::PNG;Iconv::Iconv"
)

add_library(poppler-cpp STATIC IMPORTED GLOBAL)
add_dependencies(poppler-cpp poppler-ext)

set_target_properties(poppler-cpp PROPERTIES
	IMPORTED_LOCATION "${POPPLER_INSTALL_DIR}/lib/${STATIC_LIB_PREFIX}poppler-cpp${STATIC_LIB_SUFFIX}"
	INTERFACE_INCLUDE_DIRECTORIES "${POPPLER_INSTALL_DIR}/include"
	INTERFACE_LINK_LIBRARIES "poppler"
)

# List sources.

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")
file(GLOB_RECURSE HEADERS RELATIVE ${CMAKE_SOURCE_DIR} "include/*.hpp")

add_executable(${CMAKE_PROJECT_NAME} ${HEADERS} ${SOURCES})

add_dependencies(${CMAKE_PROJECT_NAME} libjpeg-turbo-ext)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE poppler-cpp)