#include "FritzBox.hpp"
#include <QTimer>

//03.11.16 13:17:08;RING;0;015146609763;90969248;SIP1;
//03.11.16 13:17:16;CONNECT;0;1;015146609763;
//03.11.16 13:20:37;DISCONNECT;0;201;
//21.11.16 22:51:49;CALL;0;1;90969248;017624025482;SIP1;

// http://www.ip-phone-forum.de/showthread.php?t=93501

const QString FritzBox::DEFAULT_HOST_NAME( QStringLiteral( "fritz.box" ));
const FritzBox::Port FritzBox::DEFAULT_CALL_MONITOR_PORT = 1012;

//==================================================================================================

FritzBox::FritzBox( QObject *parent ) noexcept
	: QObject( parent ) {

	socket_ = new QTcpSocket( this );
	connect( socket_, qOverload< QTcpSocket::SocketError >( &QTcpSocket::error ), this, &FritzBox::onError );
	connect( socket_, &QTcpSocket::stateChanged, this, &FritzBox::stateChanged );
	connect( socket_, &QTcpSocket::readyRead, this, &FritzBox::onReadyRead );
}

//==================================================================================================

void FritzBox::connectTo( const QString &hostName, Port portNumber ) noexcept {
	hostName_ = hostName;
	portNumber_ = portNumber;

	reconnect();
}

//==================================================================================================

void FritzBox::disconnectFrom() noexcept {
	socket_->disconnectFromHost();
}

//==================================================================================================

void FritzBox::parseAndSignal( const QString &line ) {
	QStringList parts = line.split( ';' );
	QString dateTime      = parts[ 0 ];
	QString command       = parts[ 1 ];
	unsigned connectionId = parts[ 2 ].toUInt();

	if ( command == "RING" ) {
		QString caller = parts[ 3 ];
		QString callee = parts[ 4 ];
		emit phoneRinging( connectionId, caller, callee );
	}
	else if ( command == "CONNECT" ) {
		QString extension = parts[ 3 ];
		QString caller    = parts[ 4 ];
		emit phoneConnected( connectionId, caller );
	}
	else if ( command == "DISCONNECT" ) {
		QString durationSeconds = parts[ 3 ];
		emit phoneDisconnected( connectionId );
	}
	else if ( command == "CALL" ) {
		QString extension = parts[ 3 ];
		QString caller    = parts[ 4 ];
		QString callee    = parts[ 5 ];
		emit phoneCalling( connectionId, caller, callee );
	}
	else {
		emit errorOccured( QTcpSocket::SocketError::UnknownSocketError, tr( "Unknown command '%1'!" ).arg( line ));
	}
}

//==================================================================================================

void FritzBox::reconnect() {
	socket_->connectToHost( hostName_, portNumber_ );
}

//==================================================================================================

void FritzBox::onError( QTcpSocket::SocketError socketError ) {
	emit errorOccured( socketError, socket_->errorString() );

	// If we can't connect or lost the connection then try again:

	if ( socketError == QTcpSocket::SocketError::ConnectionRefusedError || socketError == QTcpSocket::SocketError::RemoteHostClosedError ) {
		QTimer::singleShot( 10000, [ = ] {
			if ( socket_->state() == QTcpSocket::SocketState::UnconnectedState )
				reconnect();
		});
	}
}

//==================================================================================================

void FritzBox::onReadyRead() {
	qint64 length;
	char buffer[ 100 ];

	if (( length = socket_->readLine( buffer, sizeof( buffer ))) != -1 ) {
		QString line = QString::fromLatin1( buffer, static_cast< int >( length )).remove( '\n' );
		parseAndSignal( line );
	}
}
