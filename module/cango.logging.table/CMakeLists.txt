include(cango)

cango_project_cpp_ho(cango.logging.table)
target_link_libraries(cango_logging_std_log INTERFACE
	cango::logging::core
)