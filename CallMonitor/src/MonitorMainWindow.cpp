#include "MonitorMainWindow.hpp"
#include <pera_software/aidkit/qt/core/Enums.hpp>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QTcpSocket>
#include <QApplication>
#include <QLineEdit>
#include <QGroupBox>
#include <QHBoxLayout>
#include <limits>
#include <QMessageBox>
#include <QCloseEvent>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>

using namespace std;
using namespace pera_software::company::qt;
using namespace pera_software::aidkit::qt;
using namespace net;

//==================================================================================================

MonitorMainWindow::MonitorMainWindow() {

	// Add the default menus:

	addFileMenu();
	addHelpMenu();

	// Prepare the hostname widget;

	hostName_ = new QLineEdit( FritzBox::DEFAULT_HOST_NAME );
	hostName_->setClearButtonEnabled( true );

	auto hostNameLabel = new QLabel( tr( "&Hostname:" ));
	hostNameLabel->setBuddy( hostName_ );

	// Prepare the port number widget:

	portNumber_ = new QSpinBox;
	portNumber_->setRange( PORT_MIN, PORT_MAX );
	portNumber_->setValue( FritzBox::DEFAULT_CALL_MONITOR_PORT );

	auto portNumberLabel = new QLabel( tr( "&Portnumber:" ));
	portNumberLabel->setBuddy( portNumber_ );

	// Prepare the phone book widget:

	phoneBookName_ = new QLineEdit;
	phoneBookName_->setClearButtonEnabled( true );

	auto phoneBookLabel = new QLabel( tr( "&Phonebook:" ));
	phoneBookLabel->setBuddy( phoneBookName_ );

	// Prepare the phone book browse widget:

	auto phoneBookBrowseButton = new QPushButton( tr( "&Browse..." ));

	// Prepare the fritz box layout:

	auto fritzBoxLayout = new QGridLayout;
	fritzBoxLayout->addWidget( hostNameLabel, 0, 0 );
	fritzBoxLayout->addWidget( hostName_, 0, 1 );
	fritzBoxLayout->addWidget( portNumberLabel, 0, 2 );
	fritzBoxLayout->addWidget( portNumber_, 0, 3 );


	fritzBoxLayout->addWidget( phoneBookLabel, 1, 0 );
	fritzBoxLayout->addWidget( phoneBookName_, 1, 1 );
	fritzBoxLayout->addWidget( phoneBookBrowseButton, 1, 2, 1, 2 );

	auto fritzBoxGroup = new QGroupBox( "FRITZ!Box" );
	fritzBoxGroup->setLayout( fritzBoxLayout );

	// Create the message(s) widget:

	messages_ = new MessagesWidget;
	messages_->setMaximumItemCount( 100 );

	auto messagesLayout = new QHBoxLayout;
	messagesLayout->addWidget( messages_ );

	auto messagesGroup = new QGroupBox( "Messages" );
	messagesGroup->setLayout( messagesLayout );

	auto centralWidgetLayout = new QVBoxLayout;
	centralWidgetLayout->addWidget( fritzBoxGroup );
	centralWidgetLayout->addWidget( messagesGroup );
	auto centralWidget = new QWidget;
	centralWidget->setLayout( centralWidgetLayout );

	// Prepare the tray icon:

	trayIcon_ = new QSystemTrayIcon( this );
	connect( trayIcon_, &QSystemTrayIcon::activated, this, &MonitorMainWindow::onTrayIconActivated );
	const QIcon fritzBoxIcon( ":/telephone-icon.png" );
	trayIcon_->setIcon( fritzBoxIcon );
	trayIcon_->setContextMenu( fileMenu() );
	trayIcon_->show();

	setCentralWidget( centralWidget );

	// Start connecting when the window is shown or the hostname or the portnumber changed:

	connect( quitAction(), &QAction::triggered, this, &MonitorMainWindow::disconnectFromFritzBox );
	connect( this, &PERAMainWindow::showed, this, &MonitorMainWindow::connectToFritzBox );
	connect( hostName_, &QLineEdit::editingFinished, this, &MonitorMainWindow::connectToFritzBox );
	connect( portNumber_, &QSpinBox::editingFinished, this, &MonitorMainWindow::connectToFritzBox );
}

//==================================================================================================

void MonitorMainWindow::connectToFritzBox() {
	if ( fritzBox_ == nullptr ) {
		fritzBox_ = new FritzBox( this );

		connect( fritzBox_, &FritzBox::errorOccured, [ = ]( QTcpSocket::SocketError, const QString &message ) {
			messages_->showError( message );
		});

		connect( fritzBox_, &FritzBox::stateChanged, [ = ]( QTcpSocket::SocketState state ) {
			if ( state == QTcpSocket::SocketState::ConnectedState )
				messages_->showInformation( tr( "Connected." ));
		});

		connect( fritzBox_, &FritzBox::phoneRinging, [ = ]( unsigned /* connectionId */, const QString &caller, const QString &callee ) {
			messages_->showInformation( tr( "Phone ringing: Caller: '%1', Callee: '%2'." ).arg( caller ).arg( callee ));
			trayIcon_->showMessage( "Incoming Call", caller );
		});

		connect( fritzBox_, &FritzBox::phoneCalling, [ = ]( unsigned /* connectionId */, const QString &caller, const QString &callee ) {
			messages_->showInformation( tr( "Phone calling: Caller: '%1', Callee: '%2'." ).arg( caller ).arg( callee ));
		});

		connect( fritzBox_, &FritzBox::phoneConnected, [ = ]( unsigned /* connectionId */, const QString &caller ) {
			messages_->showInformation( tr( "Phone connected: Caller: '%1'." ).arg( caller ));
		});

		connect( fritzBox_, &FritzBox::phoneDisconnected, [ = ]( unsigned /* connectionId */ ) {
			messages_->showInformation( tr( "Phone disconnected." ));
		});
	}
	QString hostName = hostName_->text();
	Port portNumber = static_cast< Port >( portNumber_->value() );
	if ( hostName != fritzBox_->hostName() || portNumber != fritzBox_->portNumber() ) {
		fritzBox_->disconnectFrom();
		fritzBox_->connectTo( hostName, portNumber );
	}
}

//==================================================================================================

void MonitorMainWindow::disconnectFromFritzBox() {
	if ( fritzBox_ != nullptr ) {
		fritzBox_->disconnectFrom();
		trayIcon_->hide();
	}
	close();
}

//==================================================================================================

void MonitorMainWindow::closeEvent( QCloseEvent *event ) {
	PERAMainWindow::closeEvent( event );
	if ( trayIcon_->isVisible() ) {
		QMessageBox::information( this, QApplication::applicationName(),
		tr("The program will keep running in the system tray. To terminate the program, "
			"choose <b>Quit</b> in the context menu of the system tray entry."));

		hide();
		event->ignore();
	}
}

//==================================================================================================

void MonitorMainWindow::onTrayIconActivated( QSystemTrayIcon::ActivationReason ) {
	show();
}
