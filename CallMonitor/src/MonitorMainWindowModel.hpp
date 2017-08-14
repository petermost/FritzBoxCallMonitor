#pragma once

#include "FritzBoxPhoneBook.hpp"
#include <pera_software/aidkit/cpp/optional.hpp>
#include <pera_software/aidkit/qt/core/Socket.hpp>
#include <pera_software/aidkit/qt/core/Persistable.hpp>
#include <pera_software/aidkit/qt/gui/ForwardDeclarations.hpp>
#include <QObject>
#include <chrono>

class FritzBox;
class QSettings;
class QAbstractItemModel;
class QStandardItemModel;

// TODO: Can we replace the hostNameChanged(), setHostName() and hostName() (and others) with some
// kind of property?

class MonitorMainWindowModel : public QObject, public pera_software::aidkit::qt::Persistable {
	Q_OBJECT
	public:
		MonitorMainWindowModel();

		void readSettings( QSettings *settings ) noexcept override;
		void writeSettings( QSettings *settings ) const noexcept override;

		bool isVisible() const;
		QString hostName() const;
		pera_software::aidkit::qt::Port portNumber() const;
		std::chrono::milliseconds notificationTimeout() const;
		QString phoneBookPath() const;
		QAbstractItemModel *messagesModel() const;

	signals:
		void visibleChanged( bool isVisible );
		void hostNameChanged( const QString &hostName );
		void portNumberChanged( pera_software::aidkit::qt::Port portNumber );
		void notificationTimeoutChanged( std::chrono::milliseconds timeout );
		void phoneBookPathChanged( const QString &phoneBookPath );

		void showNotification( const QString &title, const QString &message, std::chrono::milliseconds timeout );
		void showStatus( const QString &message,  std::chrono::milliseconds timeout ) const;

	public slots:
		void quit();
		void browseForPhoneBook();

		void beVisible( bool isVisible = true );
		void setHostName( const QString &hostName );
		void setPortNumber( pera_software::aidkit::qt::Port portNumber );
		void setNotificationTimeout( std::chrono::milliseconds timeout );
		void setPhoneBookPath( const QString &phoneBookPath );

	private:
		void connectToFritzBox();
		void onIncomingCall( unsigned connectionId, const QString &caller, const QString &callee );
		void onOutgoingCall( unsigned connectionId, const QString &caller, const QString &callee);
		void onPhoneConnected( unsigned connectionId, const QString &caller);

		pera_software::aidkit::cpp::optional< bool > isVisible_;
		FritzBox *fritzBox_ = nullptr;
		FritzBoxPhoneBook fritzBoxPhoneBook_;
		QString hostName_;
		pera_software::aidkit::qt::Port portNumber_ = 0;
		std::chrono::milliseconds notificationTimeout_;
		QString phoneBookPath_;
		pera_software::aidkit::qt::MessagesModel *messagesModel_;
};
