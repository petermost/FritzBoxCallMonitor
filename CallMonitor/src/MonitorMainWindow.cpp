// Copyright 2016 Peter Most, PERA Software Solutions GmbH
//
// This file is part of the CppAidKit library.
//
// CppAidKit is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// CppAidKit is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with CppAidKit. If not, see <http://www.gnu.org/licenses/>.

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

using namespace std;
using namespace avm;
using namespace pera_software::company::qt;
using namespace pera_software::aidkit::qt;

MonitorMainWindow::MonitorMainWindow() {

	addFileMenu();
	addHelpMenu();

	connect( this, &MonitorMainWindow::closed, QApplication::instance(), &QApplication::quit );
	connect( quitAction(), &QAction::triggered, [ = ] {
		disconnectFromFritzBox();
	});

	// Create the FRITZ!Box widgets:

	hostName_ = new QLineEdit;
	hostName_->setText( FritzBox::DEFAULT_HOST_NAME );

	portNumber_ = new QLineEdit;
	portNumber_->setValidator( new QIntValidator( 0, numeric_limits< FritzBox::SocketPort >::max() ));
	portNumber_->setText( QString::number( FritzBox::DEFAULT_CALL_MONITOR_PORT ));

	auto fritzBoxLayout = new QHBoxLayout;
	fritzBoxLayout->addWidget( hostName_ );
	fritzBoxLayout->addWidget( portNumber_ );

	auto fritzBoxGroup = new QGroupBox( "FRITZ!Box" );
	fritzBoxGroup->setLayout( fritzBoxLayout );

	// Create the message(s) widget:

	messages_ = new MessagesWidget;

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
	QIcon fritzBoxIcon( QPixmap( "/home/peter/SoftwareEntwicklung/FritzCallMonitor/CallMonitor/fritzLogo.svg" ));
	trayIcon_->setIcon( fritzBoxIcon );
	trayIcon_->setContextMenu( fileMenu() );
	trayIcon_->show();

	setCentralWidget( centralWidget );
}

void MonitorMainWindow::connectToFritzBox( const QString &hostName, quint16 portNumber) {
	if ( fritzBox_ == nullptr ) {
		fritzBox_ = new FritzBox( this );
		connect( fritzBox_, &FritzBox::errorOccured, [ = ]( QTcpSocket::SocketError error, const QString &message ) {
			messages_->showError( tr( "Error occured: '%1' (%2)!" ).arg( message ).arg( Enums::toString( error )));
		});

		connect( fritzBox_, &FritzBox::stateChanged, [ = ]( QTcpSocket::SocketState state ) {
			messages_->showDebug( tr( "State changed: %1." ).arg( Enums::toString( state )));
		});

		connect( fritzBox_, &FritzBox::phoneRinging, [ = ]( unsigned /* connectionId */, const QString &caller, const QString &callee ) {
			messages_->showInformation( tr( "Phone ringing: Caller: '%1', Callee: '%2'." ).arg( caller ).arg( callee ));
			trayIcon_->showMessage( "Incoming Call", tr( "Caller: %1" ).arg( caller ));
		});

		connect( fritzBox_, &FritzBox::phoneConnected, [ = ]( unsigned /* connectionId */, const QString &caller ) {
			messages_->showInformation( tr( "Phone connected: Caller: '%1'." ).arg( caller ));
		});

		connect( fritzBox_, &FritzBox::phoneDisconnected, [ = ]( unsigned /* connectionId */ ) {
			messages_->showInformation( tr( "Phone disconnected." ));
		});

		fritzBox_->connectTo( hostName, portNumber );
	}
}

void MonitorMainWindow::disconnectFromFritzBox() {
	if ( fritzBox_ != nullptr ) {
		fritzBox_->disconnectFrom();
		trayIcon_->hide();
	}
}
