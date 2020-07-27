#include "SimulatorMainWindow.hpp"
#include <CallMonitor/test/FritzBoxData.hpp>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QStatusBar>

static const QString TITLE(SimulatorMainWindow::tr("FRITZ!Box Simulator"));

//==================================================================================================

SimulatorMainWindow::SimulatorMainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	setWindowTitle(TITLE);

	auto incomingCallButton = new QPushButton(tr("Incoming Call"));
	connect(incomingCallButton, &QPushButton::clicked, this, &SimulatorMainWindow::doIncomingCall);

	auto outgoingCallButton = new QPushButton(tr("Outgoing Call"));
	connect(outgoingCallButton, &QPushButton::clicked, this, &SimulatorMainWindow::doOutgoingCall);

	auto connectPhoneButton = new QPushButton(tr("Connect Phone"));
	connect(connectPhoneButton, &QPushButton::clicked, this, &SimulatorMainWindow::doConnectPhone);

	auto disconnectPhoneButton = new QPushButton(tr("Disconnect Phone"));
	connect(disconnectPhoneButton, &QPushButton::clicked, this, &SimulatorMainWindow::doDisconnectPhone);

	auto layout = new QVBoxLayout;
	layout->addWidget(incomingCallButton);
	layout->addWidget(outgoingCallButton);
	layout->addWidget(connectPhoneButton);
	layout->addWidget(disconnectPhoneButton);

	auto centralWidget = new QWidget;
	centralWidget->setLayout(layout);

	setCentralWidget(centralWidget);

	server_ = new QTcpServer(this);
	if (!server_->listen(QHostAddress::Any, 10012)) {
		QMessageBox::critical(this, TITLE,
			tr("Unable to start the server: %1.").arg(server_->errorString()));
		server_->close();
		return;
	}
	connect(server_, &QTcpServer::newConnection, [=, this] {
		onNewConnection(server_->nextPendingConnection());
	});
	statusBar()->showMessage(tr("Socket is listening on %1.").arg(server_->serverPort()));
}

//==================================================================================================

SimulatorMainWindow::~SimulatorMainWindow()
{
	// We need to manually disconnect, because otherwise the QTcpSocket::disconnected signal gets
	// emitted when the destructor of this and the container with the sockets have already been
	// run!

	for (auto socket : sockets_) {
		socket->disconnectFromHost();
	}
}

//==================================================================================================

void SimulatorMainWindow::onNewConnection(QTcpSocket *socket)
{
	sockets_.append(socket);

	connect(socket, &QTcpSocket::readyRead, [=, this] {
		onReadyRead(socket);
	});

	connect(socket, &QTcpSocket::bytesWritten, [=, this](qint64 count) {
		onBytesWritten(socket, count);
	});

	connect(socket, &QTcpSocket::disconnected, [=, this] {
		onDisconnected(socket);
	});
}

//==================================================================================================

void SimulatorMainWindow::onDisconnected(QTcpSocket *socket)
{
	sockets_.removeAll(socket);
	socket->deleteLater();
}

//==================================================================================================

void SimulatorMainWindow::onReadyRead(QTcpSocket *)
{
}

//==================================================================================================

void SimulatorMainWindow::onBytesWritten(QTcpSocket *, qint64 /* count */)
{
}

//==================================================================================================

void SimulatorMainWindow::doIncomingCall()
{
	for (auto socket : sockets_) {
		socket->write(FritzBoxData::RING_DATA.toUtf8());
	}
}

//==================================================================================================

void SimulatorMainWindow::doOutgoingCall()
{
	for (auto socket : sockets_) {
		socket->write(FritzBoxData::CALLING_DATA.toUtf8());
	}
}

//==================================================================================================

void SimulatorMainWindow::doConnectPhone()
{
	for (auto socket : sockets_) {
		socket->write(FritzBoxData::CONNECT_DATA.toUtf8());
	}
}

//==================================================================================================

void SimulatorMainWindow::doDisconnectPhone()
{
	for (auto socket : sockets_) {
		socket->write(FritzBoxData::DISCONNECT_DATA.toUtf8());
	}
}
