include_guard()

macro(cango_init_install_info)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(module_export_name ${module_library_name}Targets)
set(module_export_file ${module_export_name}.cmake)
set(module_export_namespace ${module_namespace}::)
set(module_config_version_file ${module_library_name}ConfigVersion.cmake)
set(module_config_file_in_path ${CMAKE_CURRENT_LIST_DIR}/cmake/Config.cmake.in)
set(module_config_file_out_name ${module_library_name}Config.cmake)
set(module_header_path ${CMAKE_CURRENT_LIST_DIR}/${module_header_dir}/${module_namespace})
set(module_package_files
    ${CMAKE_CURRENT_BINARY_DIR}/${module_config_file_out_name}
    ${CMAKE_CURRENT_BINARY_DIR}/${module_config_version_file}
)

set(module_header_destination ${CMAKE_INSTALL_INCLUDEDIR})
set(module_target_destinations
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
set(module_package_destination ${CMAKE_INSTALL_LIBDIR}/cmake/${module_library_name})
endmacro()
