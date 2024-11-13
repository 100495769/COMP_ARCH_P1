cmake_minimum_required(VERSION 3.26)
project(imtool LANGUAGES CXX)

message(STATUS "Processor: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "C++ Compiler Version: ${CMAKE_SCXX_COMPILER_VERSION}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Set compiler options
#add_compile_options(-Wall -Wextra -Werror -pedantic -pedantic-errors -Wconversion -Wsign-conversion)

add_compile_options(-Wall -Wextra -Werror -Wpedantic -Werror -Wextra -Wconversion -Wsign-conversion)

#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} −march=native")

# Enable GoogleTest Library
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY "https://github.com/google/googletest"
        GIT_TAG "v1.15.2"
        GIT_SHALLOW TRUE)

FetchContent_MakeAvailable(googletest)
# Enable GSL Library
FetchContent_Declare(GSL
        GIT_REPOSITORY "https://github.com/microsoft/GSL"
        GIT_TAG v4.0.0
        GIT_SHALLOW ON
)
#FetchContent_MakeAvailable(GSL)

FetchContent_Declare(GSL
        GIT_REPOSITORY "https://github.com/microsoft/GSL"
        GIT_TAG "v4.0.0"
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(GSL)

# Run clang−tidy on the whole source tree
# Note this will slow down compilation.
# You may temporarily disable but do not forget to enable again.
set(CMAKE_CXX_CLANG_TIDY "clang-tidy-19;--format-style=file;--header-filter=.;")

# All includes relative to source tree root.
include_directories (PUBLIC .)

# Process cmake from sim and fluid directories
add_subdirectory(libraries)
add_subdirectory(imtool-aos)
add_subdirectory(imtool-soa)
# Unit tests and functional tests
enable_testing()
add_subdirectory(tests)