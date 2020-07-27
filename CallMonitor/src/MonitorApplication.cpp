#include "MonitorApplication.hpp"
#include <pera_software/aidkit/qt/core/Strings.hpp>

using namespace pera_software::aidkit::qt;
const QString MonitorApplication::NAME(QStringLiteral("FRITZ!Box CallMonitor"));

MonitorApplication::MonitorApplication(int *argc, char *argv[])
	: PERAApplication(NAME, argc, argv)
{
	translator_.load(":/translations/fritzboxcallmonitor.qm"_qs);
	installTranslator(&translator_);
}

MonitorApplication::~MonitorApplication()
{
	removeTranslator(&translator_);
}
