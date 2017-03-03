#include "MonitorMainWindow.hpp"
#include "MonitorMainWindowModel.hpp"

#include <QMenu>
#include <QTcpSocket>
#include <QApplication>
#include <QLineEdit>
#include <QGroupBox>
#include <QHBoxLayout>
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

MonitorMainWindow::MonitorMainWindow()
	: MonitorMainWindow( QSharedPointer< MonitorMainWindowModel >::create() ){
}

MonitorMainWindow::MonitorMainWindow( QSharedPointer< MonitorMainWindowModel > model )
	: model_( model ) {

	// Add the default menus:

	addFileMenu();
	addHelpMenu();

	connect( quitAction(), &QAction::triggered, model_.data(), &MonitorMainWindowModel::quit );
	connect( quitAction(), &QAction::triggered, this, &MonitorMainWindow::quit );

	// Prepare the hostname widget:

	hostName_ = new QLineEdit;
	hostName_->setClearButtonEnabled( true );

	connect( hostName_, &QLineEdit::editingFinished, [ = ] {
		model_->setHostName( hostName_->text() );
	});
	connect( model.data(), &MonitorMainWindowModel::hostNameChanged, hostName_, &QLineEdit::setText );

	auto hostNameLabel = new QLabel( tr( "&Hostname:" ));
	hostNameLabel->setBuddy( hostName_ );

	// Prepare the port number widget:

	portNumber_ = new QSpinBox;
	portNumber_->setRange( PORT_MIN, PORT_MAX );

	connect( portNumber_, &QSpinBox::editingFinished, [ = ] {
		model_->setPortNumber( static_cast< Port >( portNumber_->value() ));
	});
	connect( model.data(), &MonitorMainWindowModel::portNumberChanged, portNumber_, &QSpinBox::setValue );

	auto portNumberLabel = new QLabel( tr( "&Portnumber:" ));
	portNumberLabel->setBuddy( portNumber_ );

	// Prepare the phone book widget:

	phoneBookPath_ = new QLineEdit;
	phoneBookPath_->setClearButtonEnabled( true );

	connect( phoneBookPath_, &QLineEdit::editingFinished, model.data(), [ = ] {
		model_->setPhoneBookPath( phoneBookPath_->text() );
	});
	connect( model_.data(), &MonitorMainWindowModel::phoneBookPathChanged, phoneBookPath_, &QLineEdit::setText );

	auto phoneBookLabel = new QLabel( tr( "&Phonebook:" ));
	phoneBookLabel->setBuddy( phoneBookPath_ );

	// Prepare the phone book browse widget:

	browsePhoneBookPathButton_ = new QPushButton( tr( "&Browse..." ));

	connect( browsePhoneBookPathButton_, &QPushButton::clicked, model_.data(), &MonitorMainWindowModel::browseForPhoneBook );

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

	// Create the message(s) widget:

	messages_ = new MessagesWidget;
	messages_->setMaximumItemCount( 100 );

	connect( model_.data(), &MonitorMainWindowModel::showError, messages_, &MessagesWidget::showError );
	connect( model_.data(), &MonitorMainWindowModel::showInformation, messages_, &MessagesWidget::showInformation );

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
	const QIcon fritzBoxIcon( ":/telephone-icon.png" );
	trayIcon_->setIcon( fritzBoxIcon );
	trayIcon_->setContextMenu( fileMenu() );
	trayIcon_->show();

	connect( trayIcon_, &QSystemTrayIcon::activated, this, &MonitorMainWindow::onTrayIconActivated );
	connect( model_.data(), &MonitorMainWindowModel::showNotification, [ = ]( const QString &title, const QString &message ) {
		trayIcon_->showMessage( title, message );
	});

	setCentralWidget( centralWidget );
}

//==================================================================================================

void MonitorMainWindow::readSettings( QSettings *settings ) {
	PERAMainWindow::readSettings( settings );
	model_->readSettings( settings );
}

//==================================================================================================

void MonitorMainWindow::writeSettings( QSettings *settings ) const {
	PERAMainWindow::writeSettings( settings );
	model_->writeSettings( settings );
}

//==================================================================================================

void MonitorMainWindow::quit() {
	trayIcon_->hide();
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
