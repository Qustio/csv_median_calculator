cmake_minimum_required(VERSION 3.24)
project(csv_median_calculator)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(NOT EXISTS "${CMAKE_BINARY_DIR}/.gitignore")
    file(CONFIGURE OUTPUT ".gitignore" CONTENT "*\n")
endif()

include(FetchContent)
Set(FETCHCONTENT_QUIET FALSE)
FetchContent_Declare(
    Boost
    URL https://github.com/boostorg/boost/releases/download/boost-1.90.0/boost-1.90.0-cmake.tar.gz
    FIND_PACKAGE_ARGS COMPONENTS program_options
)
FetchContent_MakeAvailable(Boost)
FetchContent_Declare(
    tomlplusplus
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
	GIT_SHALLOW TRUE
    GIT_TAG        v3.4.0
)
FetchContent_MakeAvailable(tomlplusplus)
FetchContent_Declare(
    spdlog 
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
	GIT_SHALLOW TRUE
    GIT_TAG        v1.17.0
)
FetchContent_MakeAvailable(spdlog)
FetchContent_Declare(
    rapidcsv 
    GIT_REPOSITORY https://github.com/d99kris/rapidcsv.git
	GIT_SHALLOW TRUE
    GIT_TAG        v8.90
)
FetchContent_MakeAvailable(rapidcsv)

add_executable(cli src/main.cpp)
target_sources(cli PRIVATE src/app_config.hpp)
target_sources(cli PRIVATE src/config_parser.cpp src/config_parser.hpp)
target_link_libraries(
    cli PRIVATE
	Boost::headers
    Boost::program_options
    tomlplusplus::tomlplusplus
    spdlog::spdlog
	rapidcsv
)
target_compile_definitions(cli PRIVATE 
    $<$<CONFIG:Debug>:SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG>
)