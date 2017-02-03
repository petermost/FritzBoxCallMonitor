#include "FritzBox.hpp"

//03.11.16 13:17:08;RING;0;015146609763;90969248;SIP1;
//03.11.16 13:17:16;CONNECT;0;1;015146609763;
//03.11.16 13:20:37;DISCONNECT;0;201;
//21.11.16 22:51:49;CALL;0;1;90969248;017624025482;SIP1;

// http://www.ip-phone-forum.de/showthread.php?t=93501

namespace avm {

const QString FritzBox::DEFAULT_HOST_NAME( QStringLiteral( "fritz.box" ));
const FritzBox::SocketPort FritzBox::DEFAULT_CALL_MONITOR_PORT = 10012; // 1012;

FritzBox::FritzBox( QObject *parent )
	: QObject( parent ) {
	socket_ = new QTcpSocket( this );
}

void FritzBox::connectTo( const QString &hostName, SocketPort portNumber ) noexcept {
	constexpr auto errorSignal = static_cast< void ( QTcpSocket::* )( QTcpSocket::SocketError )>( &QTcpSocket::error );
	connect( socket_, errorSignal, [ = ]( QTcpSocket::SocketError socketError ) {
		errorOccured( socketError, socket_->errorString() );
	});
	connect( socket_, &QTcpSocket::stateChanged, this, &FritzBox::stateChanged );
	connect( socket_, &QTcpSocket::readyRead, this, &FritzBox::onReadyRead );

	socket_->connectToHost( hostName, portNumber );
}

void FritzBox::disconnectFrom() noexcept {
	socket_->disconnectFromHost();
}

void FritzBox::onReadyRead() {
	qint64 length;
	char buffer[ 100 ];

	if (( length = socket_->readLine( buffer, sizeof( buffer ))) != -1 ) {
		QString line = QString::fromLatin1( buffer, static_cast< int >( length )).remove( '\n' );
		QStringList parts = line.split( ';' );
		QString dateTime      = parts[ 0 ];
		QString command       = parts[ 1 ];
		unsigned connectionId = parts[ 2 ].toUInt();

		if ( command == QStringLiteral( "RING" )) {
			QString caller = parts[ 3 ];
			QString callee = parts[ 4 ];
			emit phoneRinging( connectionId, caller, callee );
		}
		else if ( command == QStringLiteral( "CONNECT" )) {
			QString extension = parts[ 3 ];
			QString caller    = parts[ 4 ];
			emit phoneConnected( connectionId, caller );
		}
		else if ( command == QStringLiteral( "DISCONNECT" )) {
			QString durationSeconds = parts[ 3 ];
			emit phoneDisconnected( connectionId );
		}
		else if ( command == QStringLiteral( "CALL" )) {
			QString extension = parts[ 3 ];
			QString caller    = parts[ 4 ];
			QString callee    = parts[ 5 ];
		}
		else {
			emit errorOccured( QTcpSocket::SocketError::UnknownSocketError, tr( "Unknown command '%1'!" ).arg( line ));
		}
	}
}

}
