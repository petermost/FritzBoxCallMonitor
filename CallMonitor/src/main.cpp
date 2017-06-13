#include "MonitorApplication.hpp"
#include "MonitorMainWindow.hpp"

#include <pera_software/aidkit/qt/core/IniSettings.hpp>

using namespace pera_software::aidkit::qt;

int main( int argc, char *argv[] ) {
	// qInfo() << "Using Qt build: " << QLibraryInfo::build() << endl;

	MonitorApplication application( argc, argv );

	MonitorMainWindow mainWindow;

	IniSettings settings;
	mainWindow.readSettings( &settings );

	// Visible status is stored in settings, so we don't need to explicitly show it via
	// mainWindow.show();

	auto result = application.exec();

	mainWindow.writeSettings( &settings );

	return result;
}
