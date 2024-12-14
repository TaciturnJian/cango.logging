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

if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
	set(CMAKE_BUILD_TYPE Release)
endif()

add_library(${module_library_name} INTERFACE)
add_library(${module_library_alias} ALIAS ${module_library_name})
target_include_directories(cango_logging
	INTERFACE ${module_include_expression}
)
set_target_properties(${module_library_name} PROPERTIES CXX_STANDARD 20)
if (MSVC)
	target_compile_options(${module_library_name} INTERFACE /utf-8)
endif()

if (PROJECT_IS_TOP_LEVEL)
    add_executable(${module_library_name}_test test/test.cpp)
    target_link_libraries(${module_library_name}_test PRIVATE ${module_library_alias})
    set_target_properties(${module_library_name}_test PROPERTIES CXX_STANDARD 20)
endif ()

cango_init_install_info()

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

