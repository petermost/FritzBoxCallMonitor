#include "MonitorMainWindow.hpp"

#include <pera_software/aidkit/qt/widgets/IntegerSpinBox.hpp>
#include <pera_software/aidkit/qt/widgets/MessagesView.hpp>
#include <pera_software/aidkit/cpp/stdlib.hpp>

#include <chrono>

#include <QMenu>
#include <QMenuBar>
#include <QTcpSocket>
#include <QApplication>
#include <QLineEdit>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QCloseEvent>
#include <QLabel>
#include <QPushButton>
#include <QStatusBar>

// TODO: Add what's this actions.

using namespace std;
using namespace chrono;
using namespace pera_software::company::qt;
using namespace pera_software::aidkit::qt;
using namespace pera_software::aidkit::cpp;

//==================================================================================================

MonitorMainWindow::MonitorMainWindow() {

	// Add the default menus:

	addFileMenu();
	addWindowMenu();
	addHelpMenu();
	addStatusBar();
	addTrayIcon();

	connect( quitAction(), &QAction::triggered, &model_, &MonitorMainWindowModel::quit );
	connect( quitAction(), &QAction::triggered, this, &MonitorMainWindow::quit );

	connect( &model_, &MonitorMainWindowModel::visibleChanged, this, &MonitorMainWindow::onVisible );

	// Prepare the hostname widget:

	hostName_ = new QLineEdit( model_.hostName() );
	hostName_->setClearButtonEnabled( true );

	connect( hostName_, &QLineEdit::editingFinished, [ = ] {
		model_.setHostName( hostName_->text() );
	});
	connect( &model_, &MonitorMainWindowModel::hostNameChanged, hostName_, &QLineEdit::setText );

	auto hostNameLabel = new QLabel( tr( "&Hostname:" ));
	hostNameLabel->setBuddy( hostName_ );

	// Prepare the port number widget:

	portNumber_ = new IntegerSpinBox( model_.portNumber() );
	portNumber_->setRange( PORT_MIN, PORT_MAX );

	connect( portNumber_, &IntegerSpinBox::editingFinished, [ = ] {
		model_.setPortNumber( static_cast< Port >( portNumber_->value() ));
	});
	connect( &model_, &MonitorMainWindowModel::portNumberChanged, portNumber_, &IntegerSpinBox::setValue );

	auto portNumberLabel = new QLabel( tr( "&Portnumber:" ));
	portNumberLabel->setBuddy( portNumber_ );

	// Prepare the phone book widget:

	phoneBookPath_ = new QLineEdit( model_.phoneBookPath() );
	phoneBookPath_->setClearButtonEnabled( true );

	connect( phoneBookPath_, &QLineEdit::editingFinished, &model_, [ = ] {
		model_.setPhoneBookPath( phoneBookPath_->text() );
	});
	connect( &model_, &MonitorMainWindowModel::phoneBookPathChanged, phoneBookPath_, &QLineEdit::setText );

	auto phoneBookLabel = new QLabel( tr( "&Phonebook:" ));
	phoneBookLabel->setBuddy( phoneBookPath_ );

	// Prepare the phone book browse widget:

	browsePhoneBookPathButton_ = new QPushButton( tr( "&Browse..." ));

	connect( browsePhoneBookPathButton_, &QPushButton::clicked, &model_, &MonitorMainWindowModel::browseForPhoneBook );

	// Prepare the fritz box layout:

	auto fritzBoxLayout = new QGridLayout;
	fritzBoxLayout->addWidget( hostNameLabel, 0, 0 );
	fritzBoxLayout->addWidget( hostName_, 0, 1 );
	fritzBoxLayout->addWidget( portNumberLabel, 0, 2 );
	fritzBoxLayout->addWidget( portNumber_, 0, 3 );

	fritzBoxLayout->addWidget( phoneBookLabel, 1, 0 );
	fritzBoxLayout->addWidget( phoneBookPath_, 1, 1 );
	fritzBoxLayout->addWidget( browsePhoneBookPathButton_, 1, 2, 1, 2 );

	auto fritzBoxGroup = new QGroupBox( "FRITZ!Box" );
	fritzBoxGroup->setLayout( fritzBoxLayout );

	// Prepare the notification layout:

	notificationTimeout_ = new IntegerSpinBox( int_cast< int >( model_.notificationTimeout().count() ));
	notificationTimeout_->setSuffix( tr( "ms" ));
	connect( notificationTimeout_, &IntegerSpinBox::editingFinished, [ = ] {
		model_.setNotificationTimeout( milliseconds( notificationTimeout_->value() ));
	});
	connect( &model_, &MonitorMainWindowModel::notificationTimeoutChanged, [ = ]( milliseconds timeout ) {
		notificationTimeout_->setValue( int_cast< int >( timeout.count() ));
	});

	auto notificationTimeoutLabel = new QLabel( tr( "&Timeout:" ));
	notificationTimeoutLabel->setBuddy( notificationTimeout_ );

	auto notificationLayout = new QHBoxLayout;
	notificationLayout->addWidget( notificationTimeoutLabel );
	notificationLayout->addWidget( notificationTimeout_ );

	auto notificationGroup = new QGroupBox( tr( "Notification" ));
	notificationGroup->setLayout( notificationLayout );

	// Create the message(s) widget:

	messages_ = new MessagesView;
	messages_->setModel( model_.messagesModel() );

	auto messagesLayout = new QHBoxLayout;
	messagesLayout->addWidget( messages_ );

	auto messagesGroup = new QGroupBox( tr( "Messages" ));
	messagesGroup->setLayout( messagesLayout );

	auto centralWidgetLayout = new QVBoxLayout;
	centralWidgetLayout->addWidget( fritzBoxGroup );
	centralWidgetLayout->addWidget( notificationGroup );
	centralWidgetLayout->addWidget( messagesGroup );

	auto centralWidget = new QWidget;
	centralWidget->setLayout( centralWidgetLayout );
	setCentralWidget( centralWidget );
}

//==================================================================================================

void MonitorMainWindow::addWindowMenu() {
	auto hideAction = new QAction( tr( "&Hide..." ), this );
	connect( hideAction, &QAction::triggered, [ = ] { model_.beVisible( false ); });

	auto windowMenu = new QMenu( tr( "&Window" ));
	windowMenu->addAction( hideAction );
	menuBar()->addMenu( windowMenu );
}

//==================================================================================================

void MonitorMainWindow::addStatusBar() {
	statusBar()->showMessage( QString::null );

	connect( &model_, &MonitorMainWindowModel::showStatus, [ = ]( const QString &message, milliseconds timeout ) {
		statusBar()->showMessage( message, int_cast< int >( timeout.count() ));
	});
}

//==================================================================================================

void MonitorMainWindow::addTrayIcon() {
	// Prepare the menu for the tray icon:

	auto showAction = new QAction( tr( "&Show Window" ), this );
	connect( showAction, &QAction::triggered, [ = ] { model_.beVisible( true ); });

	auto hideAction = new QAction( tr( "&Hide Window" ), this );
	connect( hideAction, &QAction::triggered, [ = ] { model_.beVisible( false ); });

	connect( &model_, &MonitorMainWindowModel::visibleChanged, [ = ]( bool isVisible ) {
		showAction->setDisabled( isVisible );
		hideAction->setEnabled( isVisible );
	});

	auto trayMenu = new QMenu( "TrayMenu", this );
	trayMenu->addAction( showAction );
	trayMenu->addAction( hideAction );
	trayMenu->addSeparator();
	trayMenu->addAction( quitAction() );

	// Prepare the tray icon:

	trayIcon_ = new QSystemTrayIcon( this );
	QIcon fritzBoxIcon( ":/telephone-icon.png" );
	trayIcon_->setIcon( fritzBoxIcon );
	trayIcon_->setContextMenu( trayMenu );
	trayIcon_->show();

	connect( trayIcon_, &QSystemTrayIcon::activated, this, &MonitorMainWindow::onTrayIconActivated );
	connect( &model_, &MonitorMainWindowModel::showNotification, [ = ]( const QString &title, const QString &message, milliseconds timeout ) {
		trayIcon_->showMessage( title, message, QSystemTrayIcon::MessageIcon::Information, int_cast< int >( timeout.count() ));
	});
}

//==================================================================================================

void MonitorMainWindow::readSettings( QSettings *settings ) noexcept {
	PERAMainWindow::readSettings( settings );
	model_.readSettings( settings );
}

//==================================================================================================

void MonitorMainWindow::writeSettings( QSettings *settings ) const noexcept {
	PERAMainWindow::writeSettings( settings );
	model_.writeSettings( settings );
}

//==================================================================================================

void MonitorMainWindow::quit() {
	close();
}

//==================================================================================================

//void MonitorMainWindow::onHide() {
//	if ( trayIcon_->isVisible() ) {
//		model_.beVisible( false );
//	}
//}

//==================================================================================================

void MonitorMainWindow::onVisible(bool isVisible) {
	setVisible(isVisible);
	if (isVisible)
		activateWindow();
}

//==================================================================================================

void MonitorMainWindow::onTrayIconActivated( QSystemTrayIcon::ActivationReason ) {
	model_.beVisible( !model_.isVisible() );
}

