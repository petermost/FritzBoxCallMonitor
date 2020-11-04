#pragma once

#include "FritzBox.hpp"
#include "FritzBoxPhoneBook.hpp"
#include "MillisecondsProperty.hpp"
#include "MonitorSettings.hpp"
#include <QObject>
#include <chrono>
#include <optional>
#include <pera_software/aidkit/qt/gui/ForwardDeclarations.hpp>
#include <pera_software/aidkit/qt/gui/MessagesModel.hpp>
#include <pera_software/aidkit/qt/properties/StringProperty.hpp>
#include <pera_software/aidkit/qt/properties/BooleanProperty.hpp>
#include <pera_software/aidkit/qt/properties/IntegerProperty.hpp>

class FritzBox;
class QSettings;
class QAbstractItemModel;

class MonitorMainWindowModel : public QObject {
	Q_OBJECT
	public:
		MonitorMainWindowModel(QSharedPointer<MonitorSettings> settings);
		~MonitorMainWindowModel() override;

		// Window properties:

		pera_software::aidkit::qt::BooleanProperty isVisible;
		pera_software::aidkit::qt::StringProperty statusMessage;

		// FritzBox properties:

		pera_software::aidkit::qt::StringProperty fritzBoxHostName;
		pera_software::aidkit::qt::IntegerProperty fritzBoxPortNumber;
		pera_software::aidkit::qt::StringProperty fritzBoxPhoneBookPath;
		MillisecondsProperty notificationTimeout;

		QAbstractItemModel *messagesModel();

	Q_SIGNALS:
		void showNotification(const QString &title, const QString &message, std::chrono::milliseconds timeout);

	public Q_SLOTS:
		void onQuit();

	private:
		void readPhoneBook(const QString &phoneBookPath);

		void onErrorOccured(QTcpSocket::SocketError, const QString &errorMessage);
		void onStateChanged(QTcpSocket::SocketState state);
		void onIncomingCall(unsigned connectionId, const QString &caller, const QString &callee);
		void onOutgoingCall(unsigned connectionId, const QString &caller, const QString &callee);
		void onPhoneConnected(unsigned connectionId, const QString &caller);
		void onPhoneDisconnected(unsigned connectionId);

		QSharedPointer<MonitorSettings> settings_;

		FritzBox *fritzBox_ = nullptr;
		FritzBoxPhoneBook fritzBoxPhoneBook_;
		pera_software::aidkit::qt::MessagesModel messagesModel_;
};
