#include "MonitorMainWindow.hpp"
#include "MonitorApplication.hpp"
#include "MonitorSettingsDialog.hpp"
#include "MonitorSettingsDialogModel.hpp"
#include "MonitorVersion.hpp"
#include "MonitorResources.hpp"
#include "MonitorMainWindowModel.hpp"

#include <pera_software/aidkit/stdlib/stdlib.hpp>
#include <pera_software/aidkit/stdlib/memory.hpp>
#include <pera_software/aidkit/qt/gui/Resources.hpp>
#include <pera_software/aidkit/qt/core/Pointer.hpp>
#include <pera_software/aidkit/qt/widgets/IntegerSpinBox.hpp>
#include <pera_software/aidkit/qt/properties/Bindings.hpp>

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

#include <chrono>

// TODO: Add what's this actions.

using namespace std;
using namespace chrono;
using namespace pera_software::aidkit::qt;
using namespace pera_software::aidkit::stdlib;


MonitorMainWindow::MonitorMainWindow(QSharedPointer<MonitorSettings> settings)
	: PERAMainWindow(settings),
	  settings_(settings),
	  trayIcon_(this)
{
	// Add the default menus:

	addFileMenu();
	addEditMenu();
	addWindowMenu();
	addHelpMenu();
	addStatusBar();

	setWindowIcon(MonitorResources::icon());

	connect(quitAction(), &QAction::triggered, this, &MonitorMainWindow::onQuit);

	// Create the message(s) widget:

	auto messagesLayout = new QHBoxLayout;
	messagesLayout->addWidget(&messages_);

	auto messagesGroup = new QGroupBox(tr("Messages"));
	messagesGroup->setLayout(messagesLayout);

	setCentralWidget(messagesGroup);
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


void MonitorMainWindow::addEditMenu()
{
	auto settingsAction = new QAction(Resources::instance().settingsIcon(), tr("&Settings..."), this);
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
}

void MonitorMainWindow::addStatusBar()
{
	statusBar()->showMessage(QString());
}

void bindValueChangedProperty(StringProperty *property, function<void (QString)> handler)
{
	handler(property->value());

	StringProperty::connect(property, &StringProperty::valueChanged, std::move(handler));
}

void MonitorMainWindow::setModel(QSharedPointer<MonitorMainWindowModel> model)
{
	model_ = std::move(model);

	connect(quitAction(), &QAction::triggered, model_.get(), &MonitorMainWindowModel::onQuit);

	connect(showAction(), &QAction::triggered, [this] {
		model_->isVisible = true;
	});
	connect(hideAction(), &QAction::triggered, [this] {
		model_->isVisible = false;
	});
	connect(&model_->isVisible, &BooleanProperty::valueChanged, [=, this](bool isVisible) {
		showAction()->setDisabled(isVisible);
		hideAction()->setEnabled(isVisible);
	});

	bindValueChangedProperty(&model_->statusMessage, [this](QString message) {
		statusBar()->showMessage(message);
	});

	connect(&trayIcon_, &MonitorTrayIcon::activated, [this] {
		model_->isVisible = !model_->isVisible;
	});

	connect(model_.get(), &MonitorMainWindowModel::showNotification, [this](const QString &title, const QString &message, milliseconds timeout) {
		trayIcon_.showMessage(title, message, MonitorTrayIcon::MessageIcon::Information, int_cast<int>(timeout.count()));
	});
	messages_.setModel(model_->messagesModel());
	bindWidgetVisibleProperty(this, &model_->isVisible);
}

void MonitorMainWindow::onQuit()
{
	close();
}

void MonitorMainWindow::onAbout()
{
	QMessageBox::about(this, QString(), tr("%1\nVersion %2").arg(MonitorApplication::NAME).arg(MonitorVersion::version()));
}


void MonitorMainWindow::onEditSettings()
{
	MonitorSettingsDialogModel dialogModel(settings_);
	dialogModel.hostName = model_->fritzBoxHostName;
	dialogModel.portNumber = model_->fritzBoxPortNumber;
	dialogModel.phoneBookPath = model_->fritzBoxPhoneBookPath;
	dialogModel.notificationTimeout = model_->notificationTimeout;

	MonitorSettingsDialog settingsDialog(settings_, this);
	settingsDialog.setModel(QSharedPointer(&dialogModel, null_deleter()));

	if (settingsDialog.exec() == QDialog::Accepted) {
		model_->fritzBoxHostName = dialogModel.hostName;
		model_->fritzBoxPortNumber = dialogModel.portNumber;
		model_->fritzBoxPhoneBookPath = dialogModel.phoneBookPath;
		model_->notificationTimeout = dialogModel.notificationTimeout;
	}
}

void MonitorMainWindow::setVisible(bool isVisible)
{
	PERAMainWindow::setVisible(isVisible);
	if (isVisible) {
		activateWindow();
		raise();
	}
}
