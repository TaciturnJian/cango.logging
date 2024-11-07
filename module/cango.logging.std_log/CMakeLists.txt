include(cango)

cango_project_cpp_lib(cango.logging.std_log)
target_link_libraries(cango_logging_std_log PUBLIC
	cango::logging::core
)
