# Create a library from the source files in this directory.

project (PluginFW)

set (${PROJECT_NAME}_source_files 
	include/Plugin.h
	include/PluginFramework.h
	include/ClientHook.h
	include/PluginManager.h
	include/SharedLibrary.h
	SharedLibrary.cpp
	PluginManager.cpp
)

include_directories (
  .
  include
)

add_library (${PROJECT_NAME} ${${PROJECT_NAME}_source_files})

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION "include")

install( TARGETS ${PROJECT_NAME}
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib
         RUNTIME DESTINATION bin)
