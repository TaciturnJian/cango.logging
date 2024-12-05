cmake_minimum_required(VERSION 3.8)

project(cango.logging.rolling)
add_library(cango_logging_rolling INTERFACE)
add_library(cango::logging::rolling ALIAS cango_logging_rolling)
target_include_directories(cango_logging_rolling INTERFACE include)
target_link_libraries(cango_logging_rolling INTERFACE
	cango::logging::core
)
