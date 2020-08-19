#include "MonitorSettingsDialogModel.hpp"
#include "FritzBox.hpp"
#include "MonitorSettings.hpp"
#include <QSettings>
#include <QStandardPaths>
#include <pera_software/aidkit/qt/core/Chrono.hpp>

using namespace std::chrono;
using namespace pera_software::aidkit::qt;

//==================================================================================================

MonitorSettingsDialogModel::MonitorSettingsDialogModel(QSharedPointer<MonitorSettingsStorage> settingsStorage, QObject *parent)
	: QObject(parent), settingsStorage_(settingsStorage)
{
	lastVisitedDirectory_ = settingsStorage_->readLastVisitedDirectory();
}

//==================================================================================================

MonitorSettingsDialogModel::~MonitorSettingsDialogModel()
{
	settingsStorage_->writeLastVisitedDirectory(lastVisitedDirectory_);
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

void MonitorSettingsDialogModel::setHostName(const QString &hostName)
{
	if (settings_.hostName != hostName) {
		settings_.hostName = hostName;
		Q_EMIT hostNameChanged(settings_.hostName);
	}
}

//==================================================================================================

void MonitorSettingsDialogModel::setPortNumber(Port portNumber)
{
	if (settings_.portNumber != portNumber) {
		settings_.portNumber = portNumber;
		Q_EMIT portNumberChanged(settings_.portNumber);
	}
}

//==================================================================================================

void MonitorSettingsDialogModel::setPhoneBookPath(const QString &phoneBookPath)
{
	if (settings_.phoneBookPath != phoneBookPath) {
		settings_.phoneBookPath = phoneBookPath;
		Q_EMIT phoneBookPathChanged(settings_.phoneBookPath);
	}
}

//==================================================================================================

void MonitorSettingsDialogModel::setLastVisitedDirectory(const QDir &directory)
{
	if (lastVisitedDirectory_ != directory) {
		lastVisitedDirectory_ = directory;
	}
}

//==================================================================================================

QDir MonitorSettingsDialogModel::lastVisitedDirectory() const
{
	return lastVisitedDirectory_;
}

//==================================================================================================

void MonitorSettingsDialogModel::setNotificationTimeout(milliseconds notificationTimeout)
{
	if (settings_.notificationTimeout != notificationTimeout) {
		settings_.notificationTimeout = notificationTimeout;
		Q_EMIT notificationTimeoutChanged(settings_.notificationTimeout);
	}
}
