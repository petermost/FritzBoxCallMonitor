#include "MonitorMainWindowModel.hpp"
#include "FritzBox.hpp"
#include "MonitorSettings.hpp"
#include <QFileDialog>
#include <QSettings>
#include <QStandardPaths>
#include <pera_software/aidkit/qt/core/Chrono.hpp>
#include <pera_software/aidkit/qt/core/Enums.hpp>

using namespace std::chrono;
using namespace pera_software::aidkit::qt;

MonitorMainWindowModel::MonitorMainWindowModel(QSharedPointer<MonitorSettings> settings)
	: settings_(settings)
{
	fritzBox_ = new FritzBox(this);

	messagesModel_.setMaximumItemCount(100);

	connect(fritzBox_, &FritzBox::errorOccured, this, &MonitorMainWindowModel::onErrorOccured);
	connect(fritzBox_, &FritzBox::stateChanged, this, &MonitorMainWindowModel::onStateChanged);
	connect(fritzBox_, &FritzBox::incomingCall, this, &MonitorMainWindowModel::onIncomingCall);
	connect(fritzBox_, &FritzBox::outgoingCall, this, &MonitorMainWindowModel::onOutgoingCall);
	connect(fritzBox_, &FritzBox::phoneConnected, this, &MonitorMainWindowModel::onPhoneConnected);
	connect(fritzBox_, &FritzBox::phoneDisconnected, this, &MonitorMainWindowModel::onPhoneDisconnected);

	connect(&fritzBoxHostName, &StringProperty::valueChanged, [this](QString newHostName) {
		fritzBox_->connectTo(newHostName, static_cast<Port>(fritzBoxPortNumber));
	});
	connect(&fritzBoxPortNumber, &IntegerProperty::valueChanged, [this](int newPortNumber) {
		fritzBox_->connectTo(fritzBoxHostName, static_cast<Port>(newPortNumber));
	});
	connect(&fritzBoxPhoneBookPath, &StringProperty::valueChanged, [this](QString newPhoneBookPath) {
		readPhoneBook(newPhoneBookPath);
	});

	// Read the properties:

	isVisible = settings_->readVisibility();
	fritzBoxHostName = settings_->readHostName();
	fritzBoxPortNumber = settings_->readPortNumber();
	fritzBoxPhoneBookPath = settings_->readPhoneBookPath();
	notificationTimeout = settings_->readNotificationTimeout();

	statusMessage = tr("Loaded settings from: '%1'").arg(settings_->fileName());
}

//==================================================================================================

MonitorMainWindowModel::~MonitorMainWindowModel()
{
	settings_->writeVisibility(isVisible);
	settings_->writeHostName(fritzBoxHostName);
	settings_->writePortNumber(static_cast<Port>(fritzBoxPortNumber));
	settings_->writePhoneBookPath(fritzBoxPhoneBookPath);
	settings_->writeNotificationTimeout(notificationTimeout);

	statusMessage = tr("Saved settings to: '%1'").arg(settings_->fileName());
}

//==================================================================================================

void MonitorMainWindowModel::onErrorOccured(QTcpSocket::SocketError, const QString &errorMessage)
{
	messagesModel_.showError(tr("Connecting to '%1:%2' failed, because: '%3'")
			.arg(fritzBox_->hostName()).arg(fritzBox_->portNumber()).arg(errorMessage));
	isVisible = true;
}

//==================================================================================================

void MonitorMainWindowModel::onStateChanged(QTcpSocket::SocketState state)
{
	if (state == QTcpSocket::SocketState::ConnectedState) {
		messagesModel_.showInformation(tr("Connected to '%1:%2'").arg(fritzBox_->hostName()).arg(fritzBox_->portNumber()));
		// Q_EMIT showInformation( Enums::toString( state ));
	}
}

//==================================================================================================

void MonitorMainWindowModel::onIncomingCall(unsigned /* connectionId */, const QString &caller, const QString &callee)
{
	QString callerName = fritzBoxPhoneBook_.findNameOrDefault(caller, caller);
	QString calleeName = fritzBoxPhoneBook_.findNameOrDefault(callee, callee);
	messagesModel_.showInformation(tr("Incoming call: Caller: '%1', Callee: '%2'.").arg(callerName).arg(calleeName));

	QString message = tr("Caller: %1\nCallee: %2").arg(callerName).arg(calleeName);
	Q_EMIT showNotification(tr("Incoming Call"), message, notificationTimeout);
}

//==================================================================================================

void MonitorMainWindowModel::onOutgoingCall(unsigned /* connectionId */, const QString &caller, const QString &callee)
{
	QString callerName = fritzBoxPhoneBook_.findNameOrDefault(caller, caller);
	QString calleeName = fritzBoxPhoneBook_.findNameOrDefault(callee, callee);
	messagesModel_.showInformation(tr("Outgoing call: Caller: '%1', Callee: '%2'.").arg(callerName).arg(calleeName));
}

//==================================================================================================

void MonitorMainWindowModel::onPhoneConnected(unsigned /* connectionId */, const QString &caller)
{
	QString callerName = fritzBoxPhoneBook_.findNameOrDefault(caller, caller);
	messagesModel_.showInformation(tr("Phone connected: Caller: '%1'.").arg(callerName));
}

//==================================================================================================

void MonitorMainWindowModel::onPhoneDisconnected(unsigned /* connectionId */)
{
	messagesModel_.showInformation(tr("Phone disconnected."));
}

void MonitorMainWindowModel::onQuit()
{
	fritzBox_->disconnectFrom();
}

//==================================================================================================

void MonitorMainWindowModel::readPhoneBook(const QString &phoneBookPath)
{
	QString errorString;
	if (fritzBoxPhoneBook_.read(phoneBookPath, &errorString)) {
		fritzBoxPhoneBook_.forEach([=, this](const QString &name, const QString &number) {
			messagesModel_.showInformation(tr("Read phone book entry for '%1' with the number: '%2'.").arg(name).arg(number));
		});
	} else {
		messagesModel_.showError(tr("Unable to read '%1' because '%2'!").arg(phoneBookPath).arg(errorString));
		isVisible = true;
	}
}

//==================================================================================================

QAbstractItemModel *MonitorMainWindowModel::messagesModel()
{
	return &messagesModel_;
}
