cmake_minimum_required(VERSION 3.21)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(ModuleInfo)
include(InstallInfo)
include(ProjectUtils)

cango_init_module_info()
cango_init_install_info()
cango_init_utils()

project(${module_project_args})
cango_add_header_only_library()
cango_configure_tester()
cango_configure_installer()
