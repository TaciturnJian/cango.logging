cmake_minimum_required(VERSION 3.16)

project(cango.logging)
include(cango)

cango_project_cxx_standard(26)

cango_project_submodule(core)
cango_project_submodule(std_log)
cango_project_submodule(rolling)
cango_project_submodule(table)

cango_cpp_ho(${PROJECT_NAME})
target_link_libraries(cango_logging INTERFACE
	cango::logging::core
	cango::logging::std_log
	cango::logging::rolling
	cango::logging::table
)

cango_cpp_ho_test(test)
