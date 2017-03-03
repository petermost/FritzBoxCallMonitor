#include "MonitorMainWindowModel.hpp"
#include "FritzBox.hpp"
#include <pera_software/aidkit/qt/core/Enums.hpp>
#include <QFileDialog>
#include <QStandardPaths>
#include <QSettings>

using namespace pera_software::aidkit::qt;
using namespace pera_software::aidkit::qt::net;

static const QString HOST_NAME_KEY( QStringLiteral( "hostName" ));
static const QString PORT_NUMBER_KEY( QStringLiteral( "portNumber" ));
static const QString PHONE_BOOK_PATH( QStringLiteral( "phoneBookPath" ));

MonitorMainWindowModel::MonitorMainWindowModel() {

	fritzBox_ = new FritzBox( this );

	connect( fritzBox_, &FritzBox::errorOccured, [ = ]( QTcpSocket::SocketError, const QString &message ) {
		emit showError( message );
	});

	connect( fritzBox_, &FritzBox::stateChanged, [ = ]( QTcpSocket::SocketState state ) {
		if ( state == QTcpSocket::SocketState::ConnectedState )
			emit showInformation( tr( "Connected." ));
//		emit showInformation( Enums::toString( state ));
	});

	connect( fritzBox_, &FritzBox::phoneRinging, [ = ]( unsigned /* connectionId */, const QString &caller, const QString &callee ) {
		emit showInformation( tr( "Phone ringing: Caller: '%1', Callee: '%2'." ).arg( caller ).arg( callee ));
		onPhoneRinging( caller, callee );
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
}

//==================================================================================================

void MonitorMainWindowModel::connectToFritzBox() {
	fritzBox_->connectTo( hostName_, portNumber_ );
}

//==================================================================================================

void MonitorMainWindowModel::onPhoneRinging( const QString &caller, const QString & /* callee */ ) {
	auto nameIterator = fritzBoxPhoneBook_.find( caller );
	if ( nameIterator != fritzBoxPhoneBook_.end() ) {
		emit showNotification( tr( "Incoming Call" ), *nameIterator );
	} else {
		emit showNotification( tr( "Incoming Call" ), caller );
	}
}

//==================================================================================================

void MonitorMainWindowModel::readSettings( QSettings *settings ) {
	setHostName( qvariant_cast< QString >( settings->value( HOST_NAME_KEY, FritzBox::DEFAULT_HOST_NAME )));
	setPortNumber( qvariant_cast< Port >( settings->value( PORT_NUMBER_KEY, FritzBox::DEFAULT_CALL_MONITOR_PORT )));
	setPhoneBookPath( qvariant_cast< QString >( settings->value( PHONE_BOOK_PATH )));
}

//==================================================================================================

void MonitorMainWindowModel::writeSettings( QSettings *settings ) const {
	settings->setValue( HOST_NAME_KEY, hostName_ );
	settings->setValue( PORT_NUMBER_KEY, portNumber_ );
	settings->setValue( PHONE_BOOK_PATH, phoneBookPath_ );
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

void MonitorMainWindowModel::setPhoneBookPath( const QString &phoneBookPath ) {
	if ( phoneBookPath != phoneBookPath_ ) {
		phoneBookPath_ = phoneBookPath;
		emit phoneBookPathChanged( phoneBookPath_ );

		fritzBoxPhoneBook_.read( phoneBookPath_ );
		for( auto entry = fritzBoxPhoneBook_.begin(); entry != fritzBoxPhoneBook_.end(); ++entry ) {
			emit showInformation( tr( "Read phone book entry for '%1' with the number: '%2'." ).arg( entry.value() ).arg( entry.key() ));
		}
	}
}

//==================================================================================================

QString MonitorMainWindowModel::phoneBookPath() const {
	return phoneBookPath_;
}
