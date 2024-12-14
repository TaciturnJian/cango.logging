
macro(cango_add_test name main)
    cango_status("配置测试程序 ${name}")
    add_executable(${name} ${main})
    target_link_libraries(${name} PRIVATE ${module_library_alias})
    set_target_properties(${name} PROPERTIES CXX_STANDARD 20)
    add_test(NAME ${name} COMMAND ${name})
endmacro()

cango_add_test(${module_library_name}_test test.cpp)
