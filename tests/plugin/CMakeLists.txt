# Create a library from the source files in this directory.

project (test_plugin)


include_directories (
  .
  ../../plugin/include
)

set (test_pluginlib_source_files 
	TestPlugin.cpp
)

set (test_pluginapp_source_files 
	main.cpp
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(LIBS_ALL dl)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

#add_definitions(-DPLUGINPATH="${CMAKE_BINARY_DIR}")
add_definitions(-DPLUGINPATH="${CMAKE_INSTALL_PREFIX}/plugins/test_plugin")


add_executable (test_pluginapp ${test_pluginapp_source_files})
target_link_libraries (test_pluginapp PluginFW ${LIBS_ALL})

SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

add_library ( test_plugin SHARED ${HEADERFILES} ${test_pluginlib_source_files} )
target_link_libraries (test_plugin PluginFW)

install( TARGETS test_pluginapp
         LIBRARY DESTINATION tests/lib
         ARCHIVE DESTINATION tests/lib
         RUNTIME DESTINATION tests/bin)
         
install( TARGETS test_plugin
         LIBRARY DESTINATION plugins/test_plugin/lib
         ARCHIVE DESTINATION plugins/test_plugin/lib
         RUNTIME DESTINATION plugins/test_plugin/bin)