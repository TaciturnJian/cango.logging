cmake_minimum_required(VERSION 3.21)

project(cango.logging)

add_subdirectory(module/cango.logging.core)
add_subdirectory(module/cango.logging.rolling)
add_subdirectory(module/cango.logging.table)

add_library(cango_logging INTERFACE)
add_library(cango::logging ALIAS cango_logging)
target_include_directories(cango_logging INTERFACE include)
target_link_libraries(cango_logging INTERFACE
	cango::logging::core
	cango::logging::rolling
	cango::logging::table
)

if (PROJECT_IS_TOP_LEVEL)
	add_executable(cango_logging_test test/test.cpp)
	target_link_libraries(cango_logging_test PRIVATE cango::logging)
	set_target_properties(cango_logging_test PROPERTIES CXX_STANDARD 20)
endif()
