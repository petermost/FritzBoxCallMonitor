#include "MonitorSettingsDialogModel.hpp"
#include "FritzBox.hpp"
#include "MonitorSettings.hpp"
#include <QSettings>
#include <QStandardPaths>
#include <pera_software/aidkit/qt/core/Chrono.hpp>

using namespace std::chrono;
using namespace pera_software::aidkit::qt;
// using namespace pera_software::aidkit::cpp;

static const QString LAST_VISITED_DIRECTORY_KEY(QStringLiteral("lastVisitedDirectory"));

//==================================================================================================

MonitorSettingsDialogModel::MonitorSettingsDialogModel(QObject *parent)
	: QObject(parent)
{
	lastVisitedDirectory_ = QStandardPaths::writableLocation(QStandardPaths::StandardLocation::HomeLocation);
}

//==================================================================================================

MonitorSettingsDialogModel::~MonitorSettingsDialogModel()
{
}

//==================================================================================================

void MonitorSettingsDialogModel::setSettings(const MonitorSettings &settings)
{
	setHostName(settings.hostName);
	setPortNumber(settings.portNumber);
	setPhoneBookPath(settings.phoneBookPath);
	setNotificationTimeout(settings.notificationTimeout);
}

//==================================================================================================

MonitorSettings MonitorSettingsDialogModel::settings() const
{
	return settings_;
}

//==================================================================================================

void MonitorSettingsDialogModel::readSettings(QSettings *settings) noexcept
{
	setLastVisitedDirectory(qvariant_cast<QString>(settings->value(LAST_VISITED_DIRECTORY_KEY,
		lastVisitedDirectory().absolutePath())));
}

//==================================================================================================

void MonitorSettingsDialogModel::writeSettings(QSettings *settings) const noexcept
{
	settings->setValue(LAST_VISITED_DIRECTORY_KEY, lastVisitedDirectory().absolutePath());
}

//==================================================================================================

void MonitorSettingsDialogModel::setHostName(const QString &hostName)
{
	if (hostName != settings_.hostName) {
		settings_.hostName = hostName;
		emit hostNameChanged(settings_.hostName);
	}
}

//==================================================================================================

void MonitorSettingsDialogModel::setPortNumber(Port portNumber)
{
	if (portNumber != settings_.portNumber) {
		settings_.portNumber = portNumber;
		emit portNumberChanged(settings_.portNumber);
	}
}

//==================================================================================================

void MonitorSettingsDialogModel::setPhoneBookPath(const QString &phoneBookPath)
{
	if (phoneBookPath != settings_.phoneBookPath) {
		settings_.phoneBookPath = phoneBookPath;
		emit phoneBookPathChanged(settings_.phoneBookPath);
	}
}

//==================================================================================================

void MonitorSettingsDialogModel::setLastVisitedDirectory(const QDir &directory)
{
	if (directory != lastVisitedDirectory_) {
		lastVisitedDirectory_ = directory;
	}
}

//==================================================================================================

QDir MonitorSettingsDialogModel::lastVisitedDirectory() const
{
	return lastVisitedDirectory_;
}

//==================================================================================================

void MonitorSettingsDialogModel::setNotificationTimeout(milliseconds timeout)
{
	if (timeout != settings_.notificationTimeout) {
		settings_.notificationTimeout = timeout;
		emit notificationTimeoutChanged(settings_.notificationTimeout);
	}
}
