cmake_minimum_required( VERSION 3.0 )
project( FritzBoxCallMonitor )

add_subdirectory( CppAidKit )

include( CppAidKit/cmake/all_options.cmake )

find_package( Qt5 COMPONENTS REQUIRED Widgets Network )

set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON )
# set( CMAKE_AUTOUIC ON )
set( CMAKE_INCLUDE_CURRENT_DIR ON )

####################################################################################################
# Setup Monitor:

set( sources
	CallMonitor/src/main.cpp
	CallMonitor/src/FritzBox.hpp
	CallMonitor/src/FritzBox.cpp
	CallMonitor/src/FritzBoxPhoneBook.hpp
	CallMonitor/src/FritzBoxPhoneBook.cpp
	CallMonitor/src/MonitorApplication.hpp
	CallMonitor/src/MonitorApplication.cpp
	CallMonitor/src/MonitorMainWindow.hpp
	CallMonitor/src/MonitorMainWindow.cpp
	CallMonitor/src/MonitorMainWindowModel.hpp
	CallMonitor/src/MonitorMainWindowModel.cpp
	CallMonitor/resources/Images.qrc
)

add_executable( fritzboxcallmonitor ${sources})
target_link_libraries( fritzboxcallmonitor AidKit Qt5::Widgets Qt5::Network )

####################################################################################################
# Setup simulator:

set( simulatorSources
	Simulator/src/main.cpp
	Simulator/src/SimulatorMainWindow.hpp
	Simulator/src/SimulatorMainWindow.cpp
	CallMonitor/src/FritzBox.hpp
	CallMonitor/src/FritzBox.cpp
	CallMonitor/test/FritzBoxTest.hpp
	CallMonitor/test/FritzBoxTest.cpp
)

add_executable( simulator ${simulatorSources})
target_link_libraries( simulator AidKit AidKitTest Qt5::Widgets Qt5::Network )

####################################################################################################
# Setup tests:

set( testSources
	CallMonitor/test/TestMain.cpp
	CallMonitor/src/FritzBox.hpp
	CallMonitor/src/FritzBox.cpp
	CallMonitor/src/FritzBoxPhoneBook.hpp
	CallMonitor/src/FritzBoxPhoneBook.cpp
	CallMonitor/test/FritzBoxTest.hpp
	CallMonitor/test/FritzBoxTest.cpp
	CallMonitor/test/FritzBoxPhoneBookTest.hpp
	CallMonitor/test/FritzBoxPhoneBookTest.cpp
	CallMonitor/test/Data.qrc
)
add_executable( fritztest ${testSources})
target_link_libraries( fritztest AidKitTest Qt5::Network )
