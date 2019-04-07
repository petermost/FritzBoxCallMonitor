#include "MonitorApplication.hpp"
#include "MonitorMainWindow.hpp"
#include <pera_software/company/qt/PERAIniSettings.hpp>

using namespace pera_software::company::qt;
using namespace pera_software::aidkit::qt;

int main( int argc, char *argv[] ) {
	// qInfo() << "Using Qt build: " << QLibraryInfo::build() << endl;

	MonitorApplication application( argc, argv );

	MonitorMainWindow mainWindow;

	PERAIniSettings settings( MonitorApplication::NAME );

	mainWindow.readSettings( &settings );

	// Visible status is stored in settings, so we don't need to explicitly show it via
	// mainWindow.show();

	auto result = application.exec();

	mainWindow.writeSettings( &settings );

	return result;
}
