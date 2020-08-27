#pragma once

#include "FritzBox.hpp"
#include "FritzBoxPhoneBook.hpp"
#include "MonitorSettings.hpp"
#include <QObject>
#include <chrono>
#include <optional>
#include <pera_software/aidkit/qt/gui/ForwardDeclarations.hpp>

class FritzBox;
class QSettings;
class QAbstractItemModel;

class MonitorMainWindowModel : public QObject {
	Q_OBJECT
	public:
		MonitorMainWindowModel(QSharedPointer<MonitorSettings> settings);
		~MonitorMainWindowModel() override;

		bool isVisible() const;
		QAbstractItemModel *messagesModel() const;

	Q_SIGNALS:
		void visibilityChanged(bool isVisible);

		void showNotification(const QString &title, const QString &message, std::chrono::milliseconds timeout);
		void showStatus(const QString &message) const;

	public Q_SLOTS:
		void onQuit();

		void setData(MonitorData data);
		void beVisible(bool isVisible = true);

	public:
		MonitorData data() const;

	private:
		void connectToFritzBox(const QString &hostName, pera_software::aidkit::qt::Port portNumber);
		void readPhoneBook(const QString &phoneBookPath);

		void onErrorOccured(QTcpSocket::SocketError, const QString &errorMessage);
		void onStateChanged(QTcpSocket::SocketState state);
		void onIncomingCall(unsigned connectionId, const QString &caller, const QString &callee);
		void onOutgoingCall(unsigned connectionId, const QString &caller, const QString &callee);
		void onPhoneConnected(unsigned connectionId, const QString &caller);
		void onPhoneDisconnected(unsigned connectionId);

		QSharedPointer<MonitorSettings> settings_;
		std::optional<bool> isVisible_;

		MonitorData data_;
		FritzBox *fritzBox_ = nullptr;
		FritzBoxPhoneBook fritzBoxPhoneBook_;
		pera_software::aidkit::qt::MessagesModel *messagesModel_;
};
