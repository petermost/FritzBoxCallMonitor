#pragma once

#include <QObject>
#include <QTcpSocket>
#include <QtGlobal>
#include <pera_software/aidkit/qt/core/Socket.hpp>

class FritzBox : public QObject {
	Q_OBJECT
	public:
		using Port = pera_software::aidkit::qt::Port;

		static const QString DEFAULT_HOST_NAME;
		static const Port DEFAULT_CALL_MONITOR_PORT;

		FritzBox(QObject *parent = nullptr) noexcept;

		void connectTo(const QString &hostName, Port portNumber) noexcept;
		void disconnectFrom() noexcept;

		void parseAndSignal(const QString &line);

		QString hostName() const noexcept
		{
			return hostName_;
		}

		Port portNumber() const noexcept
		{
			return portNumber_;
		}

	Q_SIGNALS:
		void stateChanged(QTcpSocket::SocketState state);
		void errorOccured(QTcpSocket::SocketError error, const QString &message);

		void incomingCall(unsigned connectionId, const QString &caller, const QString &callee);
		void phoneConnected(unsigned connectionId, const QString &caller);
		void phoneDisconnected(unsigned connectionId);
		void outgoingCall(unsigned connectionId, const QString &caller, const QString &callee);

	private Q_SLOTS:
		void onConnected();
		void onError(QTcpSocket::SocketError socketError);
		void onReadyRead();

	private:
		QString hostName_;
		Port portNumber_;
		QTcpSocket *socket_;

		void reconnect();
};
