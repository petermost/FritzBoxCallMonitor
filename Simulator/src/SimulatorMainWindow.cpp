#include "SimulatorMainWindow.hpp"
#include <CallMonitor/src/FritzBox.hpp>
#include <CallMonitor/test/FritzBoxTest.hpp>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QStatusBar>

//==================================================================================================

SimulatorMainWindow::SimulatorMainWindow( QWidget *parent )
	: QMainWindow( parent ) {

	auto ringPhoneButton = new QPushButton( "Ring Phone" );
	connect( ringPhoneButton, &QPushButton::clicked, this, &SimulatorMainWindow::onRingPhone );

	auto callPhoneButton = new QPushButton( "Call Phone" );
	connect( callPhoneButton, &QPushButton::clicked, this, &SimulatorMainWindow::onCallPhone );

	auto connectPhoneButton = new QPushButton( "Connect Phone" );
	connect( connectPhoneButton, &QPushButton::clicked, this, &SimulatorMainWindow::onConnectPhone );

	auto disconnectPhoneButton = new QPushButton( "Disconnect Phone" );
	connect( disconnectPhoneButton, &QPushButton::clicked, this, &SimulatorMainWindow::onDisconnectPhone );

	auto layout = new QVBoxLayout;
	layout->addWidget( ringPhoneButton );
	layout->addWidget( callPhoneButton );
	layout->addWidget( connectPhoneButton );
	layout->addWidget( disconnectPhoneButton );

	auto centralWidget = new QWidget;
	centralWidget->setLayout( layout );

	setCentralWidget( centralWidget );

	server_ = new QTcpServer( this );
	if ( !server_->listen( QHostAddress::Any, 10012 )) {
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

SimulatorMainWindow::~SimulatorMainWindow() {
	// We need to manually disconnect, because otherwise the QTcpSocket::disconnected signal gets
	// emitted when the destructor of this and the container with the sockets have already been
	// run!

	for ( auto socket : sockets_ ) {
		socket->disconnectFromHost();
	}
}

//==================================================================================================

void SimulatorMainWindow::onNewConnection( QTcpSocket *socket ) {
	sockets_.append( socket );

	connect( socket, &QTcpSocket::readyRead, [ = ] {
		onReadyRead( socket );
	});

	connect( socket, &QTcpSocket::bytesWritten, [ = ]( qint64 count ) {
		onBytesWritten( socket, count );
	});

	connect( socket, &QTcpSocket::disconnected, [ = ] {
		onDisconnected( socket );
	});
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
		socket->write( FritzBoxTest::RING_DATA );
	}
}

//==================================================================================================

void SimulatorMainWindow::onCallPhone() {
	for ( auto socket : sockets_ ) {
		socket->write( FritzBoxTest::CALLING_DATA );
	}
}

//==================================================================================================

void SimulatorMainWindow::onConnectPhone() {
	for ( auto socket : sockets_ ) {
		socket->write( FritzBoxTest::CONNECT_DATA );
	}
}

//==================================================================================================

void SimulatorMainWindow::onDisconnectPhone() {
	for ( auto socket : sockets_ ) {
		socket->write( FritzBoxTest::DISCONNECT_DATA );
	}
}

