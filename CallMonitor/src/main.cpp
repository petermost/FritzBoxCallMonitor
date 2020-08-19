#include "MonitorApplication.hpp"
#include "MonitorMainWindow.hpp"

using namespace pera_software::aidkit::qt;

int main(int argc, char *argv[])
{
	// qInfo() << "Using Qt build: " << QLibraryInfo::build() << endl;

	auto settings = QSharedPointer<MonitorSettingsStorage>::create();

	MonitorApplication application(&argc, argv);
	MonitorMainWindow mainWindow(settings);

	// Visible status is stored in settings, so we don't need to explicitly show it via
	// mainWindow.show();

	return application.exec();
}
