#include "SimulatorMainWindow.hpp"
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <CallMonitor/src/FritzBox.hpp>
#include <QVBoxLayout>
#include <QPushButton>
#include <QStatusBar>

using namespace avm;

static const char RING[]       = "29.10.16 18:23:02;RING;0;017624025482;90969248;SIP1;\n";
static const char CONNECT[]    = "03.11.16 13:17:16;CONNECT;0;1;015146609763;\n";
static const char DISCONNECT[] = "29.10.16 18:23:04;DISCONNECT;0;201;\n";

//03.11.16 13:17:08;RING;0;015146609763;90969248;SIP1;
//03.11.16 13:17:16;CONNECT;0;1;015146609763;
//03.11.16 13:20:37;DISCONNECT;0;201;

//==================================================================================================

SimulatorMainWindow::SimulatorMainWindow( QWidget *parent ) :
	QMainWindow( parent ) {

	auto ringPhoneButton = new QPushButton( "Ring Phone" );
	connect( ringPhoneButton, &QPushButton::clicked, this, &SimulatorMainWindow::onRingPhone );

	auto connectPhoneButton = new QPushButton( "Connect Phone" );
	connect( connectPhoneButton, &QPushButton::clicked, this, &SimulatorMainWindow::onConnectPhone );

	auto disconnectPhoneButton = new QPushButton( "Disconnect Phone" );
	connect( disconnectPhoneButton, &QPushButton::clicked, this, &SimulatorMainWindow::onDisconnectPhone );

	auto layout = new QVBoxLayout;
	layout->addWidget( ringPhoneButton );
	layout->addWidget( connectPhoneButton );
	layout->addWidget( disconnectPhoneButton );

	auto centralWidget = new QWidget;
	centralWidget->setLayout( layout );

	setCentralWidget( centralWidget );

	server_ = new QTcpServer( this );
	if ( !server_->listen( QHostAddress::Any, FritzBox::DEFAULT_CALL_MONITOR_PORT )) {
		QMessageBox::critical( this, "Fritz!Box Simulator",
			QString( "Unable to start the server: %1." ).arg( server_->errorString() ));
		server_->close();
		return;
	}
	connect( server_, &QTcpServer::newConnection, [ = ] {
		onNewConnection( server_->nextPendingConnection() );
	});
	statusBar()->showMessage( QString( "Socket is listening on %1.").arg( server_->serverPort() ));
}

//==================================================================================================

void SimulatorMainWindow::onNewConnection( QTcpSocket *socket ) {

	connect( socket, &QTcpSocket::readyRead, [ = ] {
		onReadyRead( socket );
	});

	connect( socket, &QTcpSocket::bytesWritten, [ = ]( qint64 count ) {
		onBytesWritten( socket, count );
	});

	connect( socket, &QTcpSocket::disconnected, [ = ] {
		onDisconnected( socket );
	});

	sockets_.append( socket );
}

//==================================================================================================

void SimulatorMainWindow::onDisconnected( QTcpSocket *socket ) {
	sockets_.removeAll( socket );
	socket->deleteLater();
}

//==================================================================================================

void SimulatorMainWindow::onReadyRead( QTcpSocket * ) {

}

//==================================================================================================

void SimulatorMainWindow::onBytesWritten( QTcpSocket *, qint64 /* count */ ) {

}

//==================================================================================================

void SimulatorMainWindow::onRingPhone() {
	for ( auto socket : sockets_ ) {
		socket->write( RING );
	}
}

//==================================================================================================

void SimulatorMainWindow::onConnectPhone() {
	for ( auto socket : sockets_ ) {
		socket->write( CONNECT );
	}
}

//==================================================================================================

void SimulatorMainWindow::onDisconnectPhone() {
	for ( auto socket : sockets_ ) {
		socket->write( DISCONNECT );
	}
}
