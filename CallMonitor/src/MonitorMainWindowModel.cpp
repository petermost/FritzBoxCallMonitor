#include "MonitorMainWindowModel.hpp"
#include "FritzBox.hpp"
#include <pera_software/aidkit/qt/core/Chrono.hpp>
#include <pera_software/aidkit/qt/core/Enums.hpp>
#include <pera_software/aidkit/qt/gui/MessagesModel.hpp>
#include <QFileDialog>
#include <QStandardPaths>
#include <QSettings>
#include "Settings.hpp"

using namespace std::chrono;
using namespace pera_software::aidkit::qt;
using namespace pera_software::aidkit::cpp;

static const QString IS_VISIBLE_KEY( QStringLiteral( "isVisible" ));


MonitorMainWindowModel::MonitorMainWindowModel()
	: hostName_(DEFAULT_HOST_NAME), portNumber_(DEFAULT_CALL_MONITOR_PORT),
	notificationTimeout_(DEFAULT_NOTIFICATION_TIMEOUT) {

	fritzBox_ = new FritzBox( this );

	messagesModel_ = new MessagesModel;
	messagesModel_->setMaximumItemCount( 100 );

	connect( fritzBox_, &FritzBox::errorOccured, [ = ]( QTcpSocket::SocketError, const QString &message ) {
		messagesModel_->showError( message );
		beVisible();
	});

	connect( fritzBox_, &FritzBox::stateChanged, [ = ]( QTcpSocket::SocketState state ) {
		if ( state == QTcpSocket::SocketState::ConnectedState )
			messagesModel_->showInformation( tr( "Connected." ));
		// emit showInformation( Enums::toString( state ));
	});

	connect( fritzBox_, &FritzBox::incomingCall, this, &MonitorMainWindowModel::onIncomingCall );

	connect( fritzBox_, &FritzBox::outgoingCall, this, &MonitorMainWindowModel::onOutgoingCall );

	connect( fritzBox_, &FritzBox::phoneConnected, this, &MonitorMainWindowModel::onPhoneConnected );

	connect( fritzBox_, &FritzBox::phoneDisconnected, [ = ]( unsigned /* connectionId */ ) {
		messagesModel_->showInformation( tr( "Phone disconnected." ));
	});

	connectToFritzBox();
}

//==================================================================================================

MonitorMainWindowModel::~MonitorMainWindowModel() {
}

//==================================================================================================

void MonitorMainWindowModel::connectToFritzBox() {
	fritzBox_->connectTo( hostName_, portNumber_ );
}

//==================================================================================================

void MonitorMainWindowModel::onIncomingCall(unsigned /* connectionId */, const QString &caller, const QString &callee ) {
	QString callerName = fritzBoxPhoneBook_.findNameOrDefault(caller, caller);
	messagesModel_->showInformation( tr( "Incoming call: Caller: '%1', Callee: '%2'." ).arg( callerName ).arg( callee ));

	emit showNotification( tr( "Incoming Call" ), callerName, notificationTimeout_ );
}

//==================================================================================================

void MonitorMainWindowModel::onOutgoingCall(unsigned /* connectionId */, const QString &caller, const QString &callee) {
	QString calleeName = fritzBoxPhoneBook_.findNameOrDefault(callee, callee);
	messagesModel_->showInformation(tr("Outgoing call: Caller: '%1', Callee: '%2'.").arg(caller).arg(calleeName));
}

//==================================================================================================

void MonitorMainWindowModel::onPhoneConnected(unsigned /* connectionId */, const QString &caller) {
	QString callerName = fritzBoxPhoneBook_.findNameOrDefault(caller, caller);
	messagesModel_->showInformation( tr( "Phone connected: Caller: '%1'." ).arg(callerName));
}

//==================================================================================================

void MonitorMainWindowModel::readSettings(QSettings *settings) noexcept {

	beVisible( qvariant_cast< bool >( settings->value( IS_VISIBLE_KEY, true )));
	setHostName( readHostName( settings ));
	setPortNumber( readPortNumber( settings ));
	setPhoneBookPath( readPhoneBookPath( settings ));
	setNotificationTimeout( readNotificationTimeout( settings ));

	emit showStatus( tr( "Loaded settings from: '%1'").arg( settings->fileName()), milliseconds(0));

	connectToFritzBox();
}

//==================================================================================================

void MonitorMainWindowModel::writeSettings(QSettings *settings) const noexcept {

	settings->setValue( IS_VISIBLE_KEY, isVisible() );
	writeHostName( settings, hostName_ );
	writePortNumber( settings, portNumber_ );
	writePhoneBookPath( settings, phoneBookPath_ );
	writeNotificationTimeout( settings, notificationTimeout_ );

	emit showStatus( tr( "Saved settings to: '%1'").arg( settings->fileName()), milliseconds(0));
}

//==================================================================================================

void MonitorMainWindowModel::quit() {
	fritzBox_->disconnectFrom();
}

//==================================================================================================

void MonitorMainWindowModel::setHostName(const QString &hostName) {
	if (hostName != hostName_) {
		hostName_ = hostName;

		connectToFritzBox();
	}
}

//==================================================================================================

void MonitorMainWindowModel::setPortNumber(FritzBox::Port portNumber) {
	if (portNumber != portNumber_) {
		portNumber_ = portNumber;

		connectToFritzBox();
	}
}

//==================================================================================================

bool MonitorMainWindowModel::isVisible() const {
//	Q_ASSERT( has_value( isVisible_ ));
	return *isVisible_;
}

//==================================================================================================

void MonitorMainWindowModel::beVisible( bool isVisible ) {
	if ( !has_value( isVisible_ ) || *isVisible_ != isVisible ) {
		isVisible_ = isVisible;
		emit visibleChanged( *isVisible_ );
	}
}

//==================================================================================================

QString MonitorMainWindowModel::hostName() const {
	return hostName_;
}

//==================================================================================================

FritzBox::Port MonitorMainWindowModel::portNumber() const {
	return portNumber_;
}

//==================================================================================================

QString MonitorMainWindowModel::phoneBookPath() const {
	return phoneBookPath_;
}

//==================================================================================================

milliseconds MonitorMainWindowModel::notificationTimeout() const {
	return notificationTimeout_;
}

//==================================================================================================

void MonitorMainWindowModel::setPhoneBookPath( const QString &phoneBookPath ) {
	if ( phoneBookPath != phoneBookPath_ ) {
		phoneBookPath_ = phoneBookPath;

		QString errorString;
		if ( fritzBoxPhoneBook_.read( phoneBookPath_, &errorString )) {
			fritzBoxPhoneBook_.forEach([ = ]( const QString &name, const QString &number ) {
				messagesModel_->showInformation( tr( "Read phone book entry for '%1' with the number: '%2'." ).arg( name ).arg( number ));
			});
		} else {
			messagesModel_->showError( tr( "Unable to read '%1' because '%2'!" ).arg( phoneBookPath ).arg( errorString ));
			beVisible();
		}
	}
}

//==================================================================================================

void MonitorMainWindowModel::setNotificationTimeout(milliseconds notificationTimeout) {
	if ( notificationTimeout != notificationTimeout_ ) {
		notificationTimeout_ = notificationTimeout;
	}
}

//==================================================================================================

QAbstractItemModel *MonitorMainWindowModel::messagesModel() const {
	return messagesModel_;
}
