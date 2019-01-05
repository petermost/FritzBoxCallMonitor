#include "MonitorApplication.hpp"

const QString MonitorApplication::NAME( QStringLiteral( "FRITZ!Box CallMonitor" ));

MonitorApplication::MonitorApplication( int &argc, char *argv[] )
	: PERAApplication( NAME, argc, argv ) {
}

MonitorApplication::~MonitorApplication() {
}
