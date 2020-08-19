#include "MonitorMainWindowModel.hpp"
#include "FritzBox.hpp"
#include "MonitorSettings.hpp"
#include <QFileDialog>
#include <QSettings>
#include <QStandardPaths>
#include <pera_software/aidkit/qt/core/Chrono.hpp>
#include <pera_software/aidkit/qt/core/Enums.hpp>
#include <pera_software/aidkit/qt/gui/MessagesModel.hpp>

using namespace std::chrono;
using namespace pera_software::aidkit::qt;

MonitorMainWindowModel::MonitorMainWindowModel(QSharedPointer<MonitorSettingsStorage> settingsStorage)
	: settingsStorage_(settingsStorage)
{
	fritzBox_ = new FritzBox(this);

	messagesModel_ = new MessagesModel;
	messagesModel_->setMaximumItemCount(100);

	connect(fritzBox_, &FritzBox::errorOccured, this, &MonitorMainWindowModel::onErrorOccured);
	connect(fritzBox_, &FritzBox::stateChanged, this, &MonitorMainWindowModel::onStateChanged);
	connect(fritzBox_, &FritzBox::incomingCall, this, &MonitorMainWindowModel::onIncomingCall);
	connect(fritzBox_, &FritzBox::outgoingCall, this, &MonitorMainWindowModel::onOutgoingCall);
	connect(fritzBox_, &FritzBox::phoneConnected, this, &MonitorMainWindowModel::onPhoneConnected);
	connect(fritzBox_, &FritzBox::phoneDisconnected, this, &MonitorMainWindowModel::onPhoneDisconnected);

	beVisible(settingsStorage_->readVisibility());
	setSettings(settingsStorage_->readSettings());

	// connectToFritzBox(settings_.hostName, settings_.portNumber);

	Q_EMIT showStatus(tr("Loaded settings from: '%1'").arg(settingsStorage_->fileName()));
}

//==================================================================================================

MonitorMainWindowModel::~MonitorMainWindowModel()
{
	settingsStorage_->writeVisibility(isVisible());
	settingsStorage_->writeSettings(settings_);

	Q_EMIT showStatus(tr("Saved settings to: '%1'").arg(settingsStorage_->fileName()));
}

//==================================================================================================

void MonitorMainWindowModel::connectToFritzBox(const QString &hostName, Port portNumber)
{
	fritzBox_->connectTo(hostName, portNumber);
}

//==================================================================================================

void MonitorMainWindowModel::onErrorOccured(QTcpSocket::SocketError, const QString &errorMessage)
{
	messagesModel_->showError(tr("Connecting to '%1:%2' failed, because: '%3'")
			.arg(fritzBox_->hostName()).arg(fritzBox_->portNumber()).arg(errorMessage));
	beVisible();
}

//==================================================================================================

void MonitorMainWindowModel::onStateChanged(QTcpSocket::SocketState state)
{
	if (state == QTcpSocket::SocketState::ConnectedState) {
		messagesModel_->showInformation(tr("Connected to '%1:%2'").arg(fritzBox_->hostName()).arg(fritzBox_->portNumber()));
		// Q_EMIT showInformation( Enums::toString( state ));
	}
}

//==================================================================================================

void MonitorMainWindowModel::onIncomingCall(unsigned /* connectionId */, const QString &caller, const QString &callee)
{
	QString callerName = fritzBoxPhoneBook_.findNameOrDefault(caller, caller);
	QString calleeName = fritzBoxPhoneBook_.findNameOrDefault(callee, callee);
	messagesModel_->showInformation(tr("Incoming call: Caller: '%1', Callee: '%2'.").arg(callerName).arg(calleeName));

	QString message = tr("%1 🡆 %2").arg(callerName).arg(calleeName);
	Q_EMIT showNotification(tr("Incoming Call"), message, settings_.notificationTimeout);
}

//==================================================================================================

void MonitorMainWindowModel::onOutgoingCall(unsigned /* connectionId */, const QString &caller, const QString &callee)
{
	QString callerName = fritzBoxPhoneBook_.findNameOrDefault(caller, caller);
	QString calleeName = fritzBoxPhoneBook_.findNameOrDefault(callee, callee);
	messagesModel_->showInformation(tr("Outgoing call: Caller: '%1', Callee: '%2'.").arg(callerName).arg(calleeName));
}

//==================================================================================================

void MonitorMainWindowModel::onPhoneConnected(unsigned /* connectionId */, const QString &caller)
{
	QString callerName = fritzBoxPhoneBook_.findNameOrDefault(caller, caller);
	messagesModel_->showInformation(tr("Phone connected: Caller: '%1'.").arg(callerName));
}

//==================================================================================================

void MonitorMainWindowModel::onPhoneDisconnected(unsigned /* connectionId */)
{
	messagesModel_->showInformation(tr("Phone disconnected."));
}

void MonitorMainWindowModel::onQuit()
{
	fritzBox_->disconnectFrom();
}

//==================================================================================================

void MonitorMainWindowModel::setSettings(MonitorSettings settings)
{
	if (settings_.hostName != settings.hostName || settings_.portNumber != settings.portNumber) {
		connectToFritzBox(settings.hostName, settings.portNumber);
	}
	if (settings_.phoneBookPath != settings.phoneBookPath) {
		readPhoneBook(settings.phoneBookPath);
	}
	settings_ = settings;
}

//==================================================================================================

MonitorSettings MonitorMainWindowModel::settings() const
{
	return settings_;
}

//==================================================================================================

bool MonitorMainWindowModel::isVisible() const
{
	return *isVisible_;
}

//==================================================================================================

void MonitorMainWindowModel::beVisible(bool isVisible)
{
	if (!isVisible_.has_value() || *isVisible_ != isVisible) {
		isVisible_ = isVisible;
		Q_EMIT visibilityChanged(*isVisible_);
	}
}

//==================================================================================================

void MonitorMainWindowModel::readPhoneBook(const QString &phoneBookPath)
{
	QString errorString;
	if (fritzBoxPhoneBook_.read(phoneBookPath, &errorString)) {
		fritzBoxPhoneBook_.forEach([=, this](const QString &name, const QString &number) {
			messagesModel_->showInformation(tr("Read phone book entry for '%1' with the number: '%2'.").arg(name).arg(number));
		});
	} else {
		messagesModel_->showError(tr("Unable to read '%1' because '%2'!").arg(phoneBookPath).arg(errorString));
		beVisible();
	}
}

//==================================================================================================

QAbstractItemModel *MonitorMainWindowModel::messagesModel() const
{
	return messagesModel_;
}
