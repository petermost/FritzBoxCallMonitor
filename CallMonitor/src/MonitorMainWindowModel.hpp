#pragma once

#include "FritzBox.hpp"
#include "FritzBoxPhoneBook.hpp"
#include <pera_software/aidkit/cpp/optional.hpp>
#include <pera_software/aidkit/qt/gui/ForwardDeclarations.hpp>
#include <pera_software/aidkit/qt/core/Persistable.hpp>
#include <QObject>
#include <chrono>

class FritzBox;
class QSettings;
class QAbstractItemModel;

class MonitorMainWindowModel : public QObject, public pera_software::aidkit::qt::Persistable {
	Q_OBJECT
	public:
		MonitorMainWindowModel();
		~MonitorMainWindowModel();

		void readSettings(QSettings *settings) noexcept override;
		void writeSettings(QSettings *settings) const noexcept override;

		bool isVisible() const;
		QAbstractItemModel *messagesModel() const;

	signals:
		void visibleChanged( bool isVisible );

		void showNotification( const QString &title, const QString &message, std::chrono::milliseconds timeout );
		void showStatus( const QString &message, std::chrono::milliseconds timeout ) const;

	public slots:
		void quit();

		void setHostName(const QString &hostName);
		void setPortNumber(FritzBox::Port portNumber);
		void setPhoneBookPath(const QString &phoneBookPath);
		void setNotificationTimeout(std::chrono::milliseconds notificationTimeout);
		void beVisible( bool isVisible = true );

		QString hostName() const;
		FritzBox::Port portNumber() const;
		QString phoneBookPath() const;
		std::chrono::milliseconds notificationTimeout() const;

	private:
		void connectToFritzBox();
		void onIncomingCall( unsigned connectionId, const QString &caller, const QString &callee );
		void onOutgoingCall( unsigned connectionId, const QString &caller, const QString &callee);
		void onPhoneConnected( unsigned connectionId, const QString &caller);

		pera_software::aidkit::cpp::optional< bool > isVisible_;

		QString hostName_;
		FritzBox::Port portNumber_;
		FritzBox *fritzBox_ = nullptr;

		QString phoneBookPath_;
		FritzBoxPhoneBook fritzBoxPhoneBook_;
		std::chrono::milliseconds notificationTimeout_;
		pera_software::aidkit::qt::MessagesModel *messagesModel_;
};
