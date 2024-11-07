include(cango)

cango_project_cpp_ho(cango.logging.rolling)
target_link_libraries(cango_logging_rolling INTERFACE
	cango::logging::core
)
