#pragma once

#include <pera_software/company/qt/PERAApplication.hpp>

class MonitorApplication : public pera_software::company::qt::PERAApplication {
	public:
		static const QString NAME;

		MonitorApplication( int &argc, char *argv[] );
};
