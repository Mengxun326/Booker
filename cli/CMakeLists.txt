include(FetchContent)

# CLI11
message(STATUS "Downloading CLI11...")

FetchContent_Declare(
	CLI11
	GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
	GIT_TAG v2.6.2
)

FetchContent_MakeAvailable(CLI11)

message(STATUS "CLI11 downloaded.")

file(GLOB_RECURSE BOOKER_CLI_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE BOOKER_CLI_HEADERS CONFIGURE_DEPENDS "include/*.hpp")

add_executable(${CLI_NAME} ${BOOKER_CLI_SOURCES} ${BOOKER_CLI_HEADERS})

target_compile_features(${CORE_NAME} PUBLIC cxx_std_17)
target_link_libraries(${CLI_NAME} PRIVATE ${CORE_NAME} CLI11::CLI11)

if(MSVC)
	target_compile_options(${CORE_NAME} PRIVATE /W4 /permissive-)
else()
	target_compile_options(${CORE_NAME} PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wshadow)
endif()

if(WIN32)
	target_compile_definitions(${CORE_NAME} PRIVATE NOMINMAX WIN32_LEAN_AND_MEAN)
endif()