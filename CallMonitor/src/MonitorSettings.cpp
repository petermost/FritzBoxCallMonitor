#include "MonitorSettings.hpp"
#include "MonitorApplication.hpp"
#include "FritzBox.hpp"
#include <QDir>
#include <QStandardPaths>
#include <pera_software/aidkit/qt/core/Chrono.hpp>

using namespace std::chrono;
using namespace pera_software::aidkit::qt;

static const QString IS_VISIBLE_KEY(QStringLiteral("isVisible"));
static const QString LAST_VISITED_DIRECTORY_KEY(QStringLiteral("lastVisitedDirectory"));
static const QString HOST_NAME_KEY(QStringLiteral("hostName"));
static const QString PORT_NUMBER_KEY(QStringLiteral("portNumber"));
static const QString PHONE_BOOK_PATH_KEY(QStringLiteral("phoneBookPath"));
static const QString NOTIFICATION_TIMEOUT_KEY(QStringLiteral("notificationTimeout"));

static const milliseconds DEFAULT_NOTIFICATION_TIMEOUT(10s);


MonitorSettings::MonitorSettings()
	: PERASettings(MonitorApplication::NAME)
{
}

bool MonitorSettings::readVisibility()
{
	return qvariant_cast<bool>(iniFile_.value(IS_VISIBLE_KEY, true));
}

void MonitorSettings::writeVisibility(bool isVisible)
{
	iniFile_.setValue(IS_VISIBLE_KEY, isVisible);
}

QDir MonitorSettings::readLastVisitedDirectory()
{
	return qvariant_cast<QString>(iniFile_.value(LAST_VISITED_DIRECTORY_KEY, QStandardPaths::writableLocation(QStandardPaths::StandardLocation::HomeLocation)));
}

void MonitorSettings::writeLastVisitedDirectory(const QDir &lastVisitedDirectory)
{
	iniFile_.setValue(LAST_VISITED_DIRECTORY_KEY, lastVisitedDirectory.absolutePath());
}

MonitorData MonitorSettings::readData()
{
	MonitorData data = {
		.hostName = qvariant_cast<QString>(iniFile_.value(HOST_NAME_KEY, FritzBox::DEFAULT_HOST_NAME)),
		.portNumber = qvariant_cast<FritzBox::Port>(iniFile_.value(PORT_NUMBER_KEY, FritzBox::DEFAULT_CALL_MONITOR_PORT)),
		.notificationTimeout = qvariant_cast<milliseconds>(iniFile_.value(NOTIFICATION_TIMEOUT_KEY, QVariant::fromValue(DEFAULT_NOTIFICATION_TIMEOUT))),
		.phoneBookPath = qvariant_cast<QString>(iniFile_.value(PHONE_BOOK_PATH_KEY))
	};
	return data;
}

void MonitorSettings::writeData(const MonitorData &data)
{
	iniFile_.setValue(HOST_NAME_KEY, data.hostName);
	iniFile_.setValue(PORT_NUMBER_KEY, data.portNumber);
	iniFile_.setValue(PHONE_BOOK_PATH_KEY, data.phoneBookPath);
	iniFile_.setValue(NOTIFICATION_TIMEOUT_KEY, QVariant::fromValue(data.notificationTimeout));
}
