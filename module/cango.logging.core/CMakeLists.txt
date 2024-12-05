cmake_minimum_required(VERSION 3.8)

project(cango.logging.core)
add_library(cango_logging_core INTERFACE)
add_library(cango::logging::core ALIAS cango_logging_core)
target_include_directories(cango_logging_core INTERFACE include)

if (MSVC)
    target_compile_options(cango_logging_core INTERFACE /utf-8)
endif()
