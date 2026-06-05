cmake_minimum_required(VERSION 3.14)

project(FileSorterProject VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(FileSorterLib
    src/Config.cpp
    src/Checksum.cpp
    src/PathUtils.cpp
    src/Logger.cpp
    src/Statistics.cpp
    src/FileSorter.cpp
)

target_include_directories(FileSorterLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(FileSorter
    src/main.cpp
)

target_link_libraries(FileSorter PRIVATE FileSorterLib)

if(MSVC)
    target_compile_options(FileSorterLib PRIVATE /utf-8)
    target_compile_options(FileSorter PRIVATE /utf-8)
endif()

include(CTest)

if(BUILD_TESTING)
    add_executable(FileSorterTests
        tests/test_main.cpp
    )

    target_include_directories(FileSorterTests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/external
    )

    target_link_libraries(FileSorterTests PRIVATE
        FileSorterLib
    )

    if(MSVC)
    target_compile_options(FileSorterTests PRIVATE /utf-8)
endif()

    add_test(NAME FileSorterTests COMMAND FileSorterTests)
endif()