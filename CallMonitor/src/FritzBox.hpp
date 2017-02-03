#pragma once

#include <QObject>
#include <QtGlobal>
#include <QTcpSocket>
#include <pera_software/aidkit/qt/net/Socket.hpp>

namespace avm {

class FritzBox : public QObject {
	Q_OBJECT
	public:
		using SocketPort = pera_software::aidkit::qt::net::Port;

		static const QString DEFAULT_HOST_NAME;
		static const SocketPort DEFAULT_CALL_MONITOR_PORT;

		FritzBox( QObject *parent = nullptr );

		void connectTo( const QString &hostName, SocketPort portNumber ) noexcept;
		void disconnectFrom() noexcept;

	signals:
		void stateChanged( QTcpSocket::SocketState state );
		void errorOccured( QTcpSocket::SocketError error, const QString &message );

		void phoneRinging( unsigned connectionId, const QString &caller, const QString &callee );
		void phoneConnected( unsigned connectionId, const QString &caller );
		void phoneDisconnected( unsigned connectionId );

	private slots:
		void onReadyRead();

	private:
		QTcpSocket *socket_;
};

}
