#include "MonitorMainWindow.hpp"
#include "MonitorApplication.hpp"
#include "MonitorSettingsDialog.hpp"

#include <pera_software/aidkit/cpp/stdlib.hpp>
#include <pera_software/aidkit/qt/gui/Resources.hpp>
#include <pera_software/aidkit/qt/widgets/IntegerSpinBox.hpp>
#include <pera_software/aidkit/qt/widgets/MessagesView.hpp>

#include <chrono>

#include <QApplication>
#include <QCloseEvent>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QStatusBar>
#include <QTcpSocket>

// TODO: Add what's this actions.

using namespace std;
using namespace chrono;
using namespace pera_software::company::qt;
using namespace pera_software::aidkit::qt;
using namespace pera_software::aidkit::cpp;

MonitorMainWindow::MonitorMainWindow()
{
	// Add the default menus:

	addFileMenu();
	addEditMenu();
	addWindowMenu();
	addHelpMenu();
	addStatusBar();
	addTrayIcon();

	connect(quitAction(), &QAction::triggered, &model_, &MonitorMainWindowModel::onQuit);
	connect(quitAction(), &QAction::triggered, this, &MonitorMainWindow::onQuit);

	connect(&model_, &MonitorMainWindowModel::visibleChanged, this, &MonitorMainWindow::onVisible);

	// Create the message(s) widget:

	messages_ = new MessagesView;
	messages_->setModel(model_.messagesModel());

	auto messagesLayout = new QHBoxLayout;
	messagesLayout->addWidget(messages_);

	auto messagesGroup = new QGroupBox(tr("Messages"));
	messagesGroup->setLayout(messagesLayout);

	setCentralWidget(messagesGroup);
}

MonitorMainWindow::~MonitorMainWindow()
{
}

void MonitorMainWindow::readSettings(QSettings *settings) noexcept
{
	PERAMainWindow::readSettings(settings);
	model_.readSettings(settings);
}

void MonitorMainWindow::writeSettings(QSettings *settings) const noexcept
{
	PERAMainWindow::writeSettings(settings);
	model_.writeSettings(settings);
}

void MonitorMainWindow::addEditMenu()
{
	auto settingsAction = new QAction(Resources::settingsIcon(), tr("&Settings..."), this);
	connect(settingsAction, &QAction::triggered, this, &MonitorMainWindow::onEditSettings);

	auto editMenu = new QMenu(tr("&Edit"), this);
	editMenu->addAction(settingsAction);
	menuBar()->addMenu(editMenu);
}

void MonitorMainWindow::addHelpMenu()
{
	auto aboutAction = new QAction(tr("&About..."), this);
	connect(aboutAction, &QAction::triggered, this, &MonitorMainWindow::onAbout);

	auto helpMenu = PERAMainWindow::addHelpMenu();
	helpMenu->addAction(aboutAction);
}

void MonitorMainWindow::addWindowMenu()
{
	auto hideAction = new QAction(tr("&Hide..."), this);
	connect(hideAction, &QAction::triggered, [=] { model_.beVisible(false); });

	auto windowMenu = new QMenu(tr("&Window"), this);
	windowMenu->addAction(hideAction);
	menuBar()->addMenu(windowMenu);
}

void MonitorMainWindow::addStatusBar()
{
	statusBar()->showMessage(QString());

	connect(&model_, &MonitorMainWindowModel::showStatus, [=](const QString &message, milliseconds timeout) {
		statusBar()->showMessage(message, int_cast<int>(timeout.count()));
	});
}

void MonitorMainWindow::addTrayIcon()
{
	// Prepare the menu for the tray icon:

	auto showAction = new QAction(tr("&Show Window"), this);
	connect(showAction, &QAction::triggered, [=] { model_.beVisible(true); });

	auto hideAction = new QAction(tr("&Hide Window"), this);
	connect(hideAction, &QAction::triggered, [=] { model_.beVisible(false); });

	connect(&model_, &MonitorMainWindowModel::visibleChanged, [=](bool isVisible) {
		showAction->setDisabled(isVisible);
		hideAction->setEnabled(isVisible);
	});

	auto trayMenu = new QMenu("TrayMenu", this);
	trayMenu->addAction(showAction);
	trayMenu->addAction(hideAction);
	trayMenu->addSeparator();
	trayMenu->addAction(quitAction());

	// Prepare the tray icon:

	trayIcon_ = new QSystemTrayIcon(this);
	QIcon fritzBoxIcon(":/telephone-icon.png");
	trayIcon_->setIcon(fritzBoxIcon);
	trayIcon_->setContextMenu(trayMenu);
	trayIcon_->show();

	connect(trayIcon_, &QSystemTrayIcon::activated, this, &MonitorMainWindow::onTrayIconActivated);
	connect(&model_, &MonitorMainWindowModel::showNotification, [=](const QString &title, const QString &message, milliseconds timeout) {
		trayIcon_->showMessage(title, message, QSystemTrayIcon::MessageIcon::Information, int_cast<int>(timeout.count()));
	});
}

void MonitorMainWindow::onQuit()
{
	close();
}

void MonitorMainWindow::onAbout()
{
	QMessageBox::about(this, tr("About %1").arg(MonitorApplication::NAME), tr("Version 1.1"));
}

void MonitorMainWindow::onEditSettings()
{
	MonitorSettingsDialog settingsDialog;
	settingsDialog.setSettings(model_.settings());
	if (settingsDialog.exec() == QDialog::Accepted) {
		model_.setSettings(settingsDialog.settings());
	}
}

//void MonitorMainWindow::onHide() {
//	if ( trayIcon_->isVisible() ) {
//		model_.beVisible( false );
//	}
//}

void MonitorMainWindow::onVisible(bool isVisible)
{
	setVisible(isVisible);
	if (isVisible)
		activateWindow();
}

void MonitorMainWindow::onTrayIconActivated(QSystemTrayIcon::ActivationReason)
{
	model_.beVisible(!model_.isVisible());
}
