#include "MonitorMainWindowModel.hpp"
#include "FritzBox.hpp"
#include <pera_software/aidkit/qt/core/Chrono.hpp>
#include <pera_software/aidkit/qt/core/Enums.hpp>
#include <pera_software/aidkit/qt/gui/MessagesModel.hpp>
#include <QFileDialog>
#include <QStandardPaths>
#include <QSettings>

using namespace std::chrono;
using namespace pera_software::aidkit::qt;
using namespace pera_software::aidkit::cpp;

static const QString IS_VISIBLE_KEY( QStringLiteral( "isVisible" ));
static const QString HOST_NAME_KEY( QStringLiteral( "hostName" ));
static const QString PORT_NUMBER_KEY( QStringLiteral( "portNumber" ));
static const QString NOTIFICATION_TIMEOUT_KEY( QStringLiteral( "notificationTimeout" ));
static const QString PHONE_BOOK_PATH( QStringLiteral( "phoneBookPath" ));

static const milliseconds DEFAULT_NOTIFICATION_TIMEOUT( seconds( 10 ));


MonitorMainWindowModel::MonitorMainWindowModel()
	: hostName_( FritzBox::DEFAULT_HOST_NAME ), portNumber_( FritzBox::DEFAULT_CALL_MONITOR_PORT ),
	notificationTimeout_( DEFAULT_NOTIFICATION_TIMEOUT ) {

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

void MonitorMainWindowModel::connectToFritzBox() {
	fritzBox_->connectTo( hostName_, portNumber_ );
}

//==================================================================================================

static QString findNameOrDefault(const FritzBoxPhoneBook &phoneBook, const QString &number,
		const QString &defaultName) {
	QString name = phoneBook.findName(number);
	if (name.isEmpty())
		name = defaultName;

	return name;
}

//==================================================================================================

void MonitorMainWindowModel::onIncomingCall(unsigned /* connectionId */, const QString &caller, const QString &callee ) {
	QString callerName = findNameOrDefault( fritzBoxPhoneBook_, caller, caller );
	messagesModel_->showInformation( tr( "Incoming call: Caller: '%1', Callee: '%2'." ).arg( callerName ).arg( callee ));
	emit showNotification( tr( "Incoming Call" ), callerName, notificationTimeout_ );
}

//==================================================================================================

void MonitorMainWindowModel::onOutgoingCall(unsigned /* connectionId */, const QString &caller, const QString &callee) {
	QString calleeName = findNameOrDefault( fritzBoxPhoneBook_, callee, callee );
	messagesModel_->showInformation(tr("Outgoing call: Caller: '%1', Callee: '%2'.").arg(caller).arg(calleeName));
}

//==================================================================================================

void MonitorMainWindowModel::onPhoneConnected(unsigned /* connectionId */, const QString &caller) {
	QString callerName = findNameOrDefault( fritzBoxPhoneBook_, caller, caller );
	messagesModel_->showInformation( tr( "Phone connected: Caller: '%1'." ).arg(callerName));
}

//==================================================================================================

void MonitorMainWindowModel::readSettings( QSettings *settings ) noexcept {
	beVisible( qvariant_cast< bool >( settings->value( IS_VISIBLE_KEY, true )));
	setHostName( qvariant_cast< QString >( settings->value( HOST_NAME_KEY, FritzBox::DEFAULT_HOST_NAME )));
	setPortNumber( qvariant_cast< Port >( settings->value( PORT_NUMBER_KEY, FritzBox::DEFAULT_CALL_MONITOR_PORT )));
	setPhoneBookPath( qvariant_cast< QString >( settings->value( PHONE_BOOK_PATH )));
	setNotificationTimeout( qvariant_cast< milliseconds >( settings->value( NOTIFICATION_TIMEOUT_KEY, QVariant::fromValue( DEFAULT_NOTIFICATION_TIMEOUT ))));

	emit showStatus( tr( "Loaded settings from: '%1'").arg( settings->fileName()), milliseconds(0));
}

//==================================================================================================

void MonitorMainWindowModel::writeSettings( QSettings *settings ) const noexcept {
	settings->setValue( IS_VISIBLE_KEY, isVisible() );
	settings->setValue( HOST_NAME_KEY, hostName_ );
	settings->setValue( PORT_NUMBER_KEY, portNumber_ );
	settings->setValue( PHONE_BOOK_PATH, phoneBookPath_ );
	settings->setValue( NOTIFICATION_TIMEOUT_KEY, QVariant::fromValue( notificationTimeout_ ));

	emit showStatus( tr( "Saved settings to: '%1'").arg( settings->fileName()), milliseconds(0));
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

bool MonitorMainWindowModel::isVisible() const {
	Q_ASSERT( has_value( isVisible_ ));
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
			fritzBoxPhoneBook_.forEach([ = ]( const QString &name, const QString &number ) {
				messagesModel_->showInformation( tr( "Read phone book entry for '%1' with the number: '%2'." ).arg( name ).arg( number ));
			});
		} else {
			messagesModel_->showError( tr( "Unable to read '%1' because '%2'!" ).arg( phoneBookPath_ ).arg( errorString ));
			beVisible();
		}
	}
}

//==================================================================================================

QString MonitorMainWindowModel::phoneBookPath() const {
	return phoneBookPath_;
}

//==================================================================================================

QAbstractItemModel *MonitorMainWindowModel::messagesModel() const {
	return messagesModel_;
}
