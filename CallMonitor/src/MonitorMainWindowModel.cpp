#include "MonitorMainWindowModel.hpp"
#include "FritzBox.hpp"
#include <pera_software/aidkit/qt/core/Chrono.hpp>
#include <pera_software/aidkit/qt/core/Enums.hpp>
#include <QFileDialog>
#include <QStandardPaths>
#include <QSettings>

using namespace std::chrono;
using namespace pera_software::aidkit::qt;

static const QString HOST_NAME_KEY( QStringLiteral( "hostName" ));
static const QString PORT_NUMBER_KEY( QStringLiteral( "portNumber" ));
static const QString NOTIFICATION_TIMEOUT_KEY( QStringLiteral( "notificationTimeout" ));
static const QString PHONE_BOOK_PATH( QStringLiteral( "phoneBookPath" ));

static const milliseconds DEFAULT_NOTIFICATION_TIMEOUT( seconds( 10 ));


MonitorMainWindowModel::MonitorMainWindowModel()
	: hostName_( FritzBox::DEFAULT_HOST_NAME ), portNumber_( FritzBox::DEFAULT_CALL_MONITOR_PORT ),
	notificationTimeout_( DEFAULT_NOTIFICATION_TIMEOUT ) {

	fritzBox_ = new FritzBox( this );

	connect( fritzBox_, &FritzBox::errorOccured, [ = ]( QTcpSocket::SocketError, const QString &message ) {
		emit errorOccured( message );
	});

	connect( fritzBox_, &FritzBox::stateChanged, [ = ]( QTcpSocket::SocketState state ) {
		if ( state == QTcpSocket::SocketState::ConnectedState )
			emit showInformation( tr( "Connected." ));
//		emit showInformation( Enums::toString( state ));
	});

	connect( fritzBox_, &FritzBox::phoneCalling, [ = ]( unsigned /* connectionId */, const QString &caller, const QString &callee ) {
		emit showInformation( tr( "Phone calling: Caller: '%1', Callee: '%2'." ).arg( caller ).arg( callee ));
	});

	connect( fritzBox_, &FritzBox::phoneConnected, [ = ]( unsigned /* connectionId */, const QString &caller ) {
		emit showInformation( tr( "Phone connected: Caller: '%1'." ).arg( caller ));
	});

	connect( fritzBox_, &FritzBox::phoneDisconnected, [ = ]( unsigned /* connectionId */ ) {
		emit showInformation( tr( "Phone disconnected." ));
	});

	connect( fritzBox_, &FritzBox::phoneRinging, [ = ]( unsigned /* connectionId */, const QString &caller, const QString &callee ) {
		onPhoneRinging( caller, callee );
	});

	connectToFritzBox();
}

//==================================================================================================

void MonitorMainWindowModel::connectToFritzBox() {
	fritzBox_->connectTo( hostName_, portNumber_ );
}

//==================================================================================================

void MonitorMainWindowModel::onPhoneRinging( const QString &caller, const QString &callee ) {

	// Do a reverse lookup of the caller:

	auto nameIterator = fritzBoxPhoneBook_.find( caller );
	QString callerName = ( nameIterator != fritzBoxPhoneBook_.end() ) ? *nameIterator : caller;

	emit showInformation( tr( "Phone ringing: Caller: '%1', Callee: '%2'." ).arg( callerName ).arg( callee ));
	emit showNotification( tr( "Incoming Call" ), callerName, notificationTimeout_ );
}

//==================================================================================================

void MonitorMainWindowModel::readSettings( QSettings *settings ) noexcept {
	setHostName( qvariant_cast< QString >( settings->value( HOST_NAME_KEY, FritzBox::DEFAULT_HOST_NAME )));
	setPortNumber( qvariant_cast< Port >( settings->value( PORT_NUMBER_KEY, FritzBox::DEFAULT_CALL_MONITOR_PORT )));
	setPhoneBookPath( qvariant_cast< QString >( settings->value( PHONE_BOOK_PATH )));
	setNotificationTimeout( qvariant_cast< milliseconds >( settings->value( NOTIFICATION_TIMEOUT_KEY, QVariant::fromValue( DEFAULT_NOTIFICATION_TIMEOUT ))));
}

//==================================================================================================

void MonitorMainWindowModel::writeSettings( QSettings *settings ) const noexcept {
	settings->setValue( HOST_NAME_KEY, hostName_ );
	settings->setValue( PORT_NUMBER_KEY, portNumber_ );
	settings->setValue( PHONE_BOOK_PATH, phoneBookPath_ );
	settings->setValue( NOTIFICATION_TIMEOUT_KEY, QVariant::fromValue( notificationTimeout_ ));
}


//==================================================================================================

void MonitorMainWindowModel::quit() {
	fritzBox_->disconnectFrom();
}

//==================================================================================================

void MonitorMainWindowModel::browseForPhoneBook() {
	QString phoneBookPath = QFileDialog::getOpenFileName( nullptr, tr( "Select exported phone book"),
		QStandardPaths::writableLocation( QStandardPaths::StandardLocation::HomeLocation ),
		tr( "Phonebooks (*.xml)"));

	if ( !phoneBookPath.isNull() )
		setPhoneBookPath( phoneBookPath );
}

//==================================================================================================

void MonitorMainWindowModel::setHostName( const QString &hostName ) {
	if ( hostName != hostName_ ) {
		hostName_ = hostName;
		emit hostNameChanged( hostName_ );

		connectToFritzBox();
	}
}

//==================================================================================================

QString MonitorMainWindowModel::hostName() const {
	return hostName_;
}

//==================================================================================================

void MonitorMainWindowModel::setPortNumber( Port portNumber ) {
	if ( portNumber != portNumber_ ) {
		portNumber_ = portNumber;
		emit portNumberChanged( portNumber_ );

		connectToFritzBox();
	}
}

//==================================================================================================

Port MonitorMainWindowModel::portNumber() const {
	return portNumber_;
}

//==================================================================================================

void MonitorMainWindowModel::setNotificationTimeout( std::chrono::milliseconds timeout ) {
	if ( timeout != notificationTimeout_ ) {
		notificationTimeout_ = timeout;
		emit notificationTimeoutChanged( notificationTimeout_ );
	}
}

//==================================================================================================

milliseconds MonitorMainWindowModel::notificationTimeout() const {
	return notificationTimeout_;
}

//==================================================================================================

void MonitorMainWindowModel::setPhoneBookPath( const QString &phoneBookPath ) {
	if ( phoneBookPath != phoneBookPath_ ) {
		phoneBookPath_ = phoneBookPath;
		emit phoneBookPathChanged( phoneBookPath_ );

		QString errorString;
		if ( fritzBoxPhoneBook_.read( phoneBookPath_, &errorString )) {
			for( auto entry = fritzBoxPhoneBook_.begin(); entry != fritzBoxPhoneBook_.end(); ++entry ) {
				emit showInformation( tr( "Read phone book entry for '%1' with the number: '%2'." ).arg( entry.value() ).arg( entry.key() ));
			}
		} else {
			emit errorOccured( tr( "Unable to read '%1' because '%2'!" ).arg( phoneBookPath_ ).arg( errorString ));
		}
	}
}

//==================================================================================================

QString MonitorMainWindowModel::phoneBookPath() const {
	return phoneBookPath_;
}
