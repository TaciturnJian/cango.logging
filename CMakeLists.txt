cmake_minimum_required(VERSION 3.21)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(ModuleInfo)
include(InstallInfo)

cango_init_module_info()

project(${module_project_name}
	HOMEPAGE_URL ${module_url}
	VERSION ${module_version}
	LANGUAGES CXX
)

option(CANGO_BE_QUIET "是否不打印状态日志" ON)
option(CANGO_BUILD_TEST "是否构建测试" OFF)

macro(cango_status message)
	if (NOT ${CANGO_BE_QUIET})
		message(STATUS "${module_project_name}> ${message}")
	endif()
endmacro()

if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
	cango_status("缺失构建类型，设置为 Release")
	set(CMAKE_BUILD_TYPE Release)
endif()

cango_status("配置头文件库 ${module_library_name}")
add_library(${module_library_name} INTERFACE)
add_library(${module_library_alias} ALIAS ${module_library_name})
target_include_directories(cango_logging
	INTERFACE ${module_include_expression}
)
set_target_properties(${module_library_name} PROPERTIES CXX_STANDARD 20)
if (MSVC)
	cango_status("为 MSVC 设置 UTF-8 编码")
	target_compile_options(${module_library_name} INTERFACE /utf-8)
endif()

if (${CANGO_BUILD_TEST})
	cango_status("配置构建测试")
	enable_testing()
	add_subdirectory(test)
endif ()

cango_init_install_info()

cango_status("配置安装头文件库 ${module_library_name}")
install(TARGETS ${module_library_name}
    EXPORT ${module_export_name}
	${module_target_destinations}
)
install(DIRECTORY ${module_header_path}
    DESTINATION ${module_header_destination}
)

write_basic_package_version_file(cango_loggingConfigVersion.cmake
	VERSION ${module_version}
	COMPATIBILITY AnyNewerVersion
)
install(EXPORT ${module_export_name}
	FILE ${module_export_file}
	NAMESPACE ${module_export_namespace}
	DESTINATION ${module_package_destination}
)
configure_file(${module_config_file_in_path} ${module_config_file_out_name} @ONLY)
install(FILES ${module_package_files}
	DESTINATION ${module_package_destination}
)

