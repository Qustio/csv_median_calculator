cmake_minimum_required(VERSION 3.24)

project(csv_median_calculator)

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
    GIT_TAG        v3.4.0
)
FetchContent_MakeAvailable(tomlplusplus)
FetchContent_Declare(
    spdlog 
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.17.0
)
FetchContent_MakeAvailable(spdlog)

add_executable(cli)

target_sources(cli PRIVATE src/main.cpp)
target_link_libraries(
    cli PRIVATE 
    Boost::program_options
    Boost::accumulators
    tomlplusplus::tomlplusplus
    spdlog::spdlog
)
