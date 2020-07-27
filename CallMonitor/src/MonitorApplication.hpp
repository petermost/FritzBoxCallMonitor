#pragma once

#include <pera_software/aidkit/qt/widgets/PERAApplication.hpp>
#include <QTranslator>

class MonitorApplication : public pera_software::aidkit::qt::PERAApplication {
	public:
		static const QString NAME;

		MonitorApplication(int *argc, char *argv[]);
		~MonitorApplication() override;

	private:
		QTranslator translator_;
};
