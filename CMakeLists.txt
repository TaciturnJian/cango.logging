cmake_minimum_required(VERSION 3.21)

project(cango.logging)

add_library(cango_logging INTERFACE)
add_library(cango::logging ALIAS cango_logging)
target_include_directories(cango_logging INTERFACE include)
set_target_properties(cango_logging PROPERTIES CXX_STANDARD 20)
if (MSVC)
	target_compile_options(cango_logging INTERFACE /utf-8)
endif()

if (PROJECT_IS_TOP_LEVEL)
	add_executable(cango_logging_test test/test.cpp)
	target_link_libraries(cango_logging_test PRIVATE cango::logging)
	set_target_properties(cango_logging_test PROPERTIES CXX_STANDARD 20)
endif()
