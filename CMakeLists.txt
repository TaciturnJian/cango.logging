cmake_minimum_required(VERSION 3.12)

project(cango.logging)

option(CANGO_LOGGING_ENABLE_TESTS "Enable tests for cango.logging" OFF)

if (NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if (NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
endif()

add_library(cango_logging INTERFACE)
add_library(cango::logging ALIAS cango_logging)
target_include_directories(cango_logging INTERFACE include)
set_target_properties(cango_logging PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
)

if (MSVC)
    target_compile_options(cango_logging INTERFACE /utf-8)
endif()

if(CANGO_LOGGING_ENABLE_TESTS)
    enable_testing()
    add_executable(availability test/availability.cpp)
    target_link_libraries(availability PRIVATE cango::logging)
    set_target_properties(availability PROPERTIES CXX_STANDARD 20)
    add_test(NAME availability COMMAND availability)
endif()
