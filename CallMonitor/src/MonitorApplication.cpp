#include "MonitorApplication.hpp"

MonitorApplication::MonitorApplication( int &argc, char *argv[] )
	: PERAApplication( argc, argv ) {

	setApplicationName( "FRITZ!Box CallMonitor" ); // This sets also the system tray entry label!
}
