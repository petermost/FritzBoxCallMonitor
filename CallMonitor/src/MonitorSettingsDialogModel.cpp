#include "MonitorSettingsDialogModel.hpp"
#include "FritzBox.hpp"
#include "MonitorSettings.hpp"
#include <QSettings>
#include <QStandardPaths>
#include <pera_software/aidkit/qt/core/Chrono.hpp>

using namespace std::chrono;
using namespace pera_software::aidkit::qt;

//==================================================================================================

MonitorSettingsDialogModel::MonitorSettingsDialogModel(QSharedPointer<MonitorSettings> settings, QObject *parent)
	: QObject(parent), settings_(settings)
{
	lastVisitedDirectory_ = settings_->readLastVisitedDirectory();
}

//==================================================================================================

MonitorSettingsDialogModel::~MonitorSettingsDialogModel()
{
	settings_->writeLastVisitedDirectory(lastVisitedDirectory_);
}

//==================================================================================================

void MonitorSettingsDialogModel::setData(const MonitorData &data)
{
	setHostName(data.hostName);
	setPortNumber(data.portNumber);
	setPhoneBookPath(data.phoneBookPath);
	setNotificationTimeout(data.notificationTimeout);
}

//==================================================================================================

MonitorData MonitorSettingsDialogModel::data() const
{
	return data_;
}

//==================================================================================================

void MonitorSettingsDialogModel::setHostName(const QString &hostName)
{
	if (data_.hostName != hostName) {
		data_.hostName = hostName;
		Q_EMIT hostNameChanged(data_.hostName);
	}
}

//==================================================================================================

void MonitorSettingsDialogModel::setPortNumber(Port portNumber)
{
	if (data_.portNumber != portNumber) {
		data_.portNumber = portNumber;
		Q_EMIT portNumberChanged(data_.portNumber);
	}
}

//==================================================================================================

void MonitorSettingsDialogModel::setPhoneBookPath(const QString &phoneBookPath)
{
	if (data_.phoneBookPath != phoneBookPath) {
		data_.phoneBookPath = phoneBookPath;
		Q_EMIT phoneBookPathChanged(data_.phoneBookPath);
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
	if (data_.notificationTimeout != notificationTimeout) {
		data_.notificationTimeout = notificationTimeout;
		Q_EMIT notificationTimeoutChanged(data_.notificationTimeout);
	}
}
