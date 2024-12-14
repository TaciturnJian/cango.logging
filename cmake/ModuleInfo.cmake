include_guard()

macro(cango_init_module_info)
set(module_name logging)
set(module_namespace cango)
set(module_type HEADER_ONLY)
set(module_version 0.1.0)
set(module_header_dir include)
set(module_url "https://github.com/TaciturnJian/cango.logging")

set(module_include_expression
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

set(module_project_name ${module_namespace}.${module_name})
set(module_library_name ${module_namespace}_${module_name})
set(module_library_alias ${module_namespace}::${module_name})
endmacro()
