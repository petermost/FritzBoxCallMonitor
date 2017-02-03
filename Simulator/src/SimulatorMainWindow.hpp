#pragma once

#include <QMainWindow>
#include <QVector>

class QTcpServer;
class QTcpSocket;
class QPushButton;

class SimulatorMainWindow : public QMainWindow {
	Q_OBJECT

	public:
		explicit SimulatorMainWindow(QWidget *parent = 0);

	private slots:
		void onRingPhone();
		void onConnectPhone();
		void onDisconnectPhone();

	private:
		void onNewConnection( QTcpSocket *socket );
		void onDisconnected( QTcpSocket *socket );
		void onReadyRead( QTcpSocket *socket );
		void onBytesWritten( QTcpSocket *socket, qint64 count );

		QTcpServer *server_;
		QVector< QTcpSocket * > sockets_;
};
