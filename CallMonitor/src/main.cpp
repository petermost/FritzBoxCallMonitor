#include <pera_software/company/qt/PERAApplication.hpp>
#include "MonitorMainWindow.hpp"
#include <QTimer>
#include "FritzBox.hpp"

using namespace avm;
using namespace pera_software::company::qt;

int main( int argc, char *argv[] ) {
	PERAApplication application( argc, argv );

	MonitorMainWindow mainWindow;
	mainWindow.show();

	QTimer::singleShot( 0, &mainWindow, [ & ] {
		mainWindow.connectToFritzBox( "localhost", FritzBox::DEFAULT_CALL_MONITOR_PORT );
	});

	return application.exec();
}
