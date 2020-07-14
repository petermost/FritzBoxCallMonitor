#include "MonitorMainWindow.hpp"
#include "MonitorApplication.hpp"
#include "MonitorSettingsDialog.hpp"
#include "MonitorTrayIcon.hpp"

#include <pera_software/aidkit/stdlib/stdlib.hpp>
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
#include <QAction>

// TODO: Add what's this actions.

using namespace std;
using namespace chrono;
using namespace pera_software::company::qt;
using namespace pera_software::aidkit::qt;
using namespace pera_software::aidkit::stdlib;


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

QAction *MonitorMainWindow::showAction()
{
	if (showAction_ == nullptr) {
		showAction_ = new QAction(tr("&Show Window"));
	}
	return showAction_;
}

QAction *MonitorMainWindow::hideAction()
{
	if (hideAction_ == nullptr) {
		hideAction_ = new QAction(tr("&Hide Window"));
	}
	return hideAction_;
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
	auto windowMenu = new QMenu(tr("&Window"), this);
	windowMenu->addAction(hideAction());
	menuBar()->addMenu(windowMenu);

	connect(showAction(), &QAction::triggered, [this] {
		model_.beVisible(true);
	});
	connect(hideAction(), &QAction::triggered, [this] {
		model_.beVisible(false);
	});
	connect(&model_, &MonitorMainWindowModel::visibleChanged, [=, this](bool isVisible) {
		showAction()->setDisabled(isVisible);
		hideAction()->setEnabled(isVisible);
	});
}

void MonitorMainWindow::addStatusBar()
{
	statusBar()->showMessage(QString());

	connect(&model_, &MonitorMainWindowModel::showStatus, [=, this](const QString &message) {
		statusBar()->showMessage(message);
	});
}

void MonitorMainWindow::addTrayIcon()
{
	trayIcon_ = new MonitorTrayIcon(this);

	connect(trayIcon_, &MonitorTrayIcon::activated, [this] {
		model_.beVisible(!model_.isVisible());
	});

	connect(&model_, &MonitorMainWindowModel::showNotification, [this](const QString &title, const QString &message, milliseconds timeout) {
		trayIcon_->showMessage(title, message, MonitorTrayIcon::MessageIcon::Information, int_cast<int>(timeout.count()));
	});
}

void MonitorMainWindow::onQuit()
{
	close();
}

void MonitorMainWindow::onAbout()
{
	QMessageBox::about(this, tr("About %1").arg(MonitorApplication::NAME), tr("Version 1.4.0"));
}

void MonitorMainWindow::onEditSettings()
{
	MonitorSettingsDialog settingsDialog;
	settingsDialog.setSettings(model_.settings());
	if (settingsDialog.exec() == QDialog::Accepted) {
		model_.setSettings(settingsDialog.settings());
	}
}

void MonitorMainWindow::onVisible(bool isVisible)
{
	setVisible(isVisible);
	if (isVisible) {
		activateWindow();
		raise();
	}
}
