#include "MonitorSettingsDialogModel.hpp"
#include "FritzBox.hpp"
#include "Settings.hpp"
#include <pera_software/aidkit/qt/core/Chrono.hpp>
#include <QSettings>
#include <QStandardPaths>

using namespace std::chrono;
using namespace pera_software::aidkit::qt;
// using namespace pera_software::aidkit::cpp;

static const QString LAST_VISITED_DIRECTORY_KEY( QStringLiteral( "lastVisitedDirectory" ));

//==================================================================================================

MonitorSettingsDialogModel::MonitorSettingsDialogModel(QObject *parent)
		: QObject(parent) {

	hostName_ = DEFAULT_HOST_NAME;
	portNumber_ = DEFAULT_CALL_MONITOR_PORT;
	notificationTimeout_ = DEFAULT_NOTIFICATION_TIMEOUT;
	lastVisitedDirectory_ = QStandardPaths::writableLocation( QStandardPaths::StandardLocation::HomeLocation );
}

//==================================================================================================

MonitorSettingsDialogModel::~MonitorSettingsDialogModel() {
}

//==================================================================================================

void MonitorSettingsDialogModel::readSettings(QSettings *settings) noexcept {
	setLastVisitedDirectory( qvariant_cast< QString >( settings->value( LAST_VISITED_DIRECTORY_KEY,
		lastVisitedDirectory().absolutePath() )));
}

//==================================================================================================

void MonitorSettingsDialogModel::writeSettings(QSettings *settings) const noexcept {
	settings->setValue( LAST_VISITED_DIRECTORY_KEY, lastVisitedDirectory().absolutePath() );
}

//==================================================================================================

void MonitorSettingsDialogModel::setHostName(const QString &hostName) {
	if ( hostName != hostName_ ) {
		hostName_ = hostName;
		emit hostNameChanged( hostName_ );
	}
}

//==================================================================================================

QString MonitorSettingsDialogModel::hostName() const {
	return hostName_;
}

//==================================================================================================

void MonitorSettingsDialogModel::setPortNumber(Port portNumber) {
	if ( portNumber != portNumber_ ) {
		portNumber_ = portNumber;
		emit portNumberChanged( portNumber_ );
	}
}

//==================================================================================================

Port MonitorSettingsDialogModel::portNumber() const {
	return portNumber_;
}

//==================================================================================================

void MonitorSettingsDialogModel::setPhoneBookPath(const QString &phoneBookPath) {
	if ( phoneBookPath != phoneBookPath_ ) {
		phoneBookPath_ = phoneBookPath;
		emit phoneBookPathChanged( phoneBookPath_ );
	}
}

//==================================================================================================

QString MonitorSettingsDialogModel::phoneBookPath() const {
	return phoneBookPath_;
}

//==================================================================================================

void MonitorSettingsDialogModel::setLastVisitedDirectory(const QDir &directory) {
	if (directory != lastVisitedDirectory_ ) {
		lastVisitedDirectory_ = directory;
	}
}

//==================================================================================================

QDir MonitorSettingsDialogModel::lastVisitedDirectory() const {
	return lastVisitedDirectory_;
}

//==================================================================================================

void MonitorSettingsDialogModel::setNotificationTimeout(milliseconds timeout) {
	if ( timeout != notificationTimeout_ ) {
		notificationTimeout_ = timeout;
		emit notificationTimeoutChanged( notificationTimeout_ );
	}

}

//==================================================================================================

milliseconds MonitorSettingsDialogModel::notificationTimeout() const {
	return notificationTimeout_;
}
