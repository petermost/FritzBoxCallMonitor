#include "MonitorSettings.hpp"
#include "MonitorMainWindow.hpp"
#include "MonitorApplication.hpp"
#include "MonitorMainWindowModel.hpp"

#include <pera_software/aidkit/stdlib/memory.hpp>
#include <pera_software/aidkit/qt/core/Pointer.hpp>

using namespace pera_software::aidkit::qt;
using namespace pera_software::aidkit::stdlib;

int main(int argc, char *argv[])
{
	// qInfo() << "Using Qt build: " << QLibraryInfo::build() << endl;

	MonitorApplication application(&argc, argv);

	MonitorSettings monitorSettings;
	QSharedPointer settingsPtr(&monitorSettings, null_deleter());

	MonitorMainWindow mainWindow(settingsPtr);

	MonitorMainWindowModel mainWindowModel(settingsPtr);
	mainWindow.setModel(QSharedPointer(&mainWindowModel, null_deleter()));

	// Visible status is stored in settings, so we don't need to explicitly show it via
	// mainWindow.show();

	return application.exec();
}
