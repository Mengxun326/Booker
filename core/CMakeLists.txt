include(FetchContent)

# RarLib
message(STATUS "Downloading RarLib...")

FetchContent_Declare(
	RarLib
	GIT_REPOSITORY https://github.com/DostLeFan/RarLib.git
	GIT_TAG v0.5.0
)

FetchContent_MakeAvailable(RarLib)

message(STATUS "RarLib downloaded.")

# unarr
message(STATUS "Downloading unarr...")

FetchContent_Declare(
	unarr_src
	GIT_REPOSITORY https://github.com/selmf/unarr.git
	GIT_TAG v1.1.1
)

FetchContent_GetProperties(unarr_src)

if(NOT unarr_src_POPULATED)
	FetchContent_Populate(unarr_src)
	
	file(GLOB_RECURSE UNARR_SOURCES "${unarr_src_SOURCE_DIR}/*/*.c")
	list(FILTER UNARR_SOURCES EXCLUDE REGEX ".*/test/.*")
	
	add_library(unarr STATIC ${UNARR_SOURCES})
	
	target_include_directories(unarr PUBLIC ${unarr_src_SOURCE_DIR})
	target_link_libraries(unarr PUBLIC ZLIB::ZLIB)
	target_compile_definitions(unarr PRIVATE HAVE_ZLIB)
	
	set_target_properties(unarr PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

message(STATUS "unarr downloaded.")

# imageinfo
message(STATUS "Downloading imageinfo...")

FetchContent_Declare(
	imageinfo
	GIT_REPOSITORY https://github.com/xiaozhuai/imageinfo.git
	GIT_TAG 0a97e63249d447107137519b2d20d52be0a598b2
)

FetchContent_MakeAvailable(imageinfo)

message(STATUS "imageinfo downloaded.")

# libharu
message(STATUS "Downloading libharu...")

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
	libharu
	GIT_REPOSITORY https://github.com/libharu/libharu.git
	GIT_TAG v2.4.6
)

FetchContent_MakeAvailable(libharu)

message(STATUS "libharu downloaded.")

file(GLOB_RECURSE BOOKER_CORE_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE BOOKER_CORE_HEADERS CONFIGURE_DEPENDS "include/*.hpp")

add_library(${CORE_NAME} ${BOOKER_CORE_SOURCES} ${BOOKER_CORE_HEADERS})
add_library(booker::core ALIAS ${CORE_NAME})

target_include_directories(${CORE_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${CORE_NAME} PRIVATE ${libharu_SOURCE_DIR}/include ${libharu_BINARY_DIR}/include)
target_compile_features(${CORE_NAME} PUBLIC cxx_std_17)
target_link_libraries(${CORE_NAME} PRIVATE poppler-cpp poppler ZLIB::ZLIB minizip PNG::PNG JPEG::JPEG Freetype::Freetype Iconv::Iconv RarLib::RarLib unarr imageinfo hpdf)

add_dependencies(${CORE_NAME} poppler-ext libjpeg-turbo-ext)

if(MSVC)
	target_compile_options(${CORE_NAME} PRIVATE /W4 /permissive-)
else()
	target_compile_options(${CORE_NAME} PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wshadow)
endif()

if(WIN32)
	target_compile_definitions(${CORE_NAME} PRIVATE NOMINMAX WIN32_LEAN_AND_MEAN)
endif()

set_target_properties(${CORE_NAME} PROPERTIES OUTPUT_NAME "${CORE_NAME}")