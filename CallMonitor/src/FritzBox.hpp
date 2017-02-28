#pragma once

#include <QObject>
#include <QtGlobal>
#include <QTcpSocket>
#include <pera_software/aidkit/qt/net/Socket.hpp>

class FritzBox : public QObject {
	Q_OBJECT
	public:
		using Port = pera_software::aidkit::qt::net::Port;

		static const QString DEFAULT_HOST_NAME;
		static const Port DEFAULT_CALL_MONITOR_PORT;

		FritzBox( QObject *parent = nullptr ) noexcept;

		void connectTo( const QString &hostName, Port portNumber ) noexcept;
		void disconnectFrom() noexcept;

		void parseAndSignal( const QString &line );

		QString hostName() const noexcept {
			return hostName_;
		}

		Port portNumber() const noexcept {
			return portNumber_;
		}

	signals:
		void stateChanged( QTcpSocket::SocketState state );
		void errorOccured( QTcpSocket::SocketError error, const QString &message );

		void phoneRinging( unsigned connectionId, const QString &caller, const QString &callee );
		void phoneConnected( unsigned connectionId, const QString &caller );
		void phoneDisconnected( unsigned connectionId );
		void phoneCalling( unsigned connectionId, const QString &caller, const QString &callee );

	private slots:
		void onError( QTcpSocket::SocketError socketError );
		void onReadyRead();

	private:
		QString hostName_;
		Port portNumber_;
		QTcpSocket *socket_;

		void reconnect();
};
