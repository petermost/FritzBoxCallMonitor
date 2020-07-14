cmake_minimum_required(VERSION 3.16)
project(FritzBoxCallMonitor)

add_subdirectory(CppAidKit)
include(CppAidKit/cmake/all_options.cmake)

find_package(Qt5 COMPONENTS REQUIRED Widgets Network Sql Test)
# Alternative but not recommended!: find_package(GTest)
if (NOT TARGET gtest AND NOT TARGET gtest_main)
	add_subdirectory(/usr/src/googletest googletest)
endif()

# set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTORCC ON)
# set(CMAKE_AUTOUIC ON)
# set(CMAKE_INCLUDE_CURRENT_DIR ON)

####################################################################################################
# Setup Monitor:

add_library(fritzboxlib STATIC
	CallMonitor/src/FritzBox.hpp
	CallMonitor/src/FritzBox.cpp
	CallMonitor/src/FritzBoxPhoneBook.hpp
	CallMonitor/src/FritzBoxPhoneBook.cpp
	CallMonitor/src/PhoneBook.hpp
	CallMonitor/src/PhoneBook.cpp
)
set_default_target_options(fritzboxlib)

target_include_directories(fritzboxlib
	PUBLIC
		.
)

target_link_libraries(fritzboxlib
	PUBLIC
		AidKit
		Qt5::Network
		Qt5::Sql
)

add_executable(fritzboxcallmonitor
	CallMonitor/src/main.cpp
	CallMonitor/src/MonitorApplication.hpp
	CallMonitor/src/MonitorApplication.cpp
	CallMonitor/src/MonitorTrayIcon.hpp
	CallMonitor/src/MonitorTrayIcon.cpp
	CallMonitor/src/MonitorMainWindow.hpp
	CallMonitor/src/MonitorMainWindow.cpp
	CallMonitor/src/MonitorMainWindowModel.hpp
	CallMonitor/src/MonitorMainWindowModel.cpp
	CallMonitor/src/MonitorSettingsDialog.hpp
	CallMonitor/src/MonitorSettingsDialog.cpp
	CallMonitor/src/MonitorSettingsDialogModel.hpp
	CallMonitor/src/MonitorSettingsDialogModel.cpp
	CallMonitor/src/MonitorSettings.cpp
	CallMonitor/src/MonitorSettings.hpp
	CallMonitor/resources/Images.qrc
)
set_default_target_options(fritzboxcallmonitor)

target_link_libraries(fritzboxcallmonitor
	fritzboxlib
	AidKit
	Qt5::Widgets
	Qt5::Network
)

####################################################################################################
# Setup simulator:

add_executable(simulator
	Simulator/src/main.cpp
	Simulator/src/SimulatorMainWindow.hpp
	Simulator/src/SimulatorMainWindow.cpp
	CallMonitor/test/FritzBoxData.hpp
	CallMonitor/test/FritzBoxData.cpp
)

set_default_target_options(simulator)

target_link_libraries(simulator
	fritzboxlib
	AidKit
	Qt5::Widgets
	Qt5::Network
)

####################################################################################################
# Setup tests:

add_executable(fritztest
	CallMonitor/test/FritzBoxTest.cpp
	CallMonitor/test/FritzBoxPhoneBookTest.cpp
	CallMonitor/test/PhoneBookTest.cpp
	CallMonitor/test/FritzBoxData.hpp
	CallMonitor/test/FritzBoxData.cpp
	CallMonitor/test/Data.qrc
)
set_default_target_options(fritztest)

target_link_libraries(fritztest
	fritzboxlib
	Qt5::Test
	gtest
	gtest_main
)
