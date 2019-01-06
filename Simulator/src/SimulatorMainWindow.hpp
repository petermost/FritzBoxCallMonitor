#pragma once

#include <QVector>
#include <QPointer>
#include <QMainWindow>

class QTcpServer;
class QTcpSocket;
class QPushButton;

class SimulatorMainWindow : public QMainWindow {
	Q_OBJECT

	public:
		explicit SimulatorMainWindow(QWidget *parent = nullptr);
		~SimulatorMainWindow() override;

	private slots:
		void doIncomingCall();
		void doOutgoingCall();
		void doConnectPhone();
		void doDisconnectPhone();

	private:
		void onNewConnection( QTcpSocket *socket );
		void onDisconnected( QTcpSocket *socket );
		void onReadyRead( QTcpSocket *socket );
		void onBytesWritten( QTcpSocket *socket, qint64 count );

		QTcpServer *server_;
		QVector< QTcpSocket * > sockets_;
};
