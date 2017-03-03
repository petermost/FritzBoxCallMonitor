#include "MonitorApplication.hpp"
#include "MonitorMainWindow.hpp"

using namespace pera_software::company::qt;

int main( int argc, char *argv[] ) {
	// qInfo() << "Using Qt build: " << QLibraryInfo::build() << endl;

	MonitorApplication application( argc, argv );

	MonitorMainWindow mainWindow;
	mainWindow.show();

	return application.exec();
}
