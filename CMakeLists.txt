cmake_minimum_required(VERSION 3.16)
project(FritzBoxCallMonitor VERSION 1.7.0)

add_subdirectory(CppAidKit)
include(CppAidKit/cmake/AidKit.cmake)

find_package(Qt5 REQUIRED
	COMPONENTS
		Widgets
		Network
		Test
		LinguistTools
		# Sql
)

add_googletest_subdirectory()

# set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTORCC ON)
# set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Prepare version information:
configure_file(CallMonitor/src/MonitorVersion.h.in MonitorVersion.h)

# Embed translation files as a resource:
# From "The missing example to use cmake qt5_create_translation" (https://gist.github.com/giraldeau/546ba5512a74dfe9d8ea0862d66db412)

configure_file(CallMonitor/translations/Translations.qrc Translations.qrc COPYONLY)
qt5_create_translation(TRANSLATION_FILES ${CMAKE_CURRENT_SOURCE_DIR} CallMonitor/translations/fritzboxcallmonitor_de.ts)

####################################################################################################
# Setup Monitor:

add_library(fritzboxlib STATIC
	CallMonitor/src/FritzBox.hpp
	CallMonitor/src/FritzBox.cpp
	CallMonitor/src/FritzBoxPhoneBook.hpp
	CallMonitor/src/FritzBoxPhoneBook.cpp
	# CallMonitor/src/PhoneBook.hpp
	# CallMonitor/src/PhoneBook.cpp
)
set_default_target_options(fritzboxlib)

target_link_libraries(fritzboxlib
	PUBLIC
		AidKit
		Qt5::Network
		# Qt5::Sql
)
add_executable(fritzboxcallmonitor WIN32
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
	CallMonitor/src/MillisecondsProperty.hpp
	CallMonitor/src/MillisecondsProperty.cpp
	CallMonitor/src/MonitorSettingsDialogModel.hpp
	CallMonitor/src/MonitorSettingsDialogModel.cpp
	CallMonitor/src/MonitorSettings.cpp
	CallMonitor/src/MonitorSettings.hpp
	CallMonitor/src/MonitorResources.hpp
	CallMonitor/src/MonitorResources.cpp
	CallMonitor/src/MonitorVersion.h.in
	${CMAKE_CURRENT_BINARY_DIR}/MonitorVersion.h
	CallMonitor/src/MonitorVersion.hpp
	CallMonitor/src/MonitorVersion.cpp
	CallMonitor/resources/Images.qrc
	${TRANSLATION_FILES}
	${CMAKE_CURRENT_BINARY_DIR}/Translations.qrc
	CallMonitor/resources/Icon.rc
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

if (TARGET AidKitTest)
	add_executable(fritztest
		CallMonitor/test/FritzBoxTest.cpp
		CallMonitor/test/FritzBoxPhoneBookTest.cpp
		CallMonitor/test/FritzBoxData.hpp
		CallMonitor/test/FritzBoxData.cpp
		CallMonitor/test/Data.qrc
		# CallMonitor/test/PhoneBookTest.cpp
	)
	set_default_target_options(fritztest)

	target_link_libraries(fritztest
		fritzboxlib
		Qt5::Test
		AidKitTest
	)
endif()
