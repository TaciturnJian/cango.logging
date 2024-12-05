cmake_minimum_required(VERSION 3.8)

project(cango.logging.table)
add_library(cango_logging_table INTERFACE)
add_library(cango::logging::table ALIAS cango_logging_table)
target_include_directories(cango_logging_table INTERFACE include)
target_link_libraries(cango_logging_table INTERFACE
	cango::logging::core
)