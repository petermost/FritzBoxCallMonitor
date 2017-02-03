cmake_minimum_required( VERSION 3.0 )

project( FritzCallMonitor )

include( ../CppAidKit/cmake/all_options.cmake )
include( ../CppAidKit/cmake/aidkit_options.cmake )

find_package( Qt5 COMPONENTS Widgets Network Svg )

set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON )
set( CMAKE_AUTOUIC ON )
set( CMAKE_INCLUDE_CURRENT_DIR ON )

set( CMAKE_INSTALL_RPATH "$ORIGIN" )
set( CMAKE_BUILD_WITH_INSTALL_RPATH ON )

####################################################################################################
# Setup Monitor:

set( sources
	CallMonitor/src/main.cpp
	CallMonitor/src/FritzBox.hpp
	CallMonitor/src/FritzBox.cpp
	CallMonitor/src/MonitorMainWindow.hpp
	CallMonitor/src/MonitorMainWindow.cpp
)

add_executable( fritzcallmonitor ${sources})
target_link_libraries( fritzcallmonitor AidKit Qt5::Widgets Qt5::Network Qt5::Svg )

####################################################################################################
# Setup tests:

set( testSources
	CallMonitor/test/TestMain.cpp
)
add_executable( fritztest ${testSources})
target_link_libraries( fritztest AidKitTest )

####################################################################################################
# Setup simulator:

set( simulatorSources
	Simulator/src/main.cpp
	CallMonitor/src/FritzBox.hpp
	CallMonitor/src/FritzBox.cpp
	Simulator/src/SimulatorMainWindow.hpp
	Simulator/src/SimulatorMainWindow.cpp
)

add_executable( simulator ${simulatorSources})
target_link_libraries( simulator AidKit Qt5::Widgets Qt5::Network )

