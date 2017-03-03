#pragma once

#include "FritzBoxPhoneBook.hpp"
#include <pera_software/aidkit/qt/net/Socket.hpp>
#include <pera_software/aidkit/qt/core/Persistable.hpp>
#include <QObject>

class FritzBox;
class QSettings;

// TODO: Can we replace the hostNameChanged(), setHostName() and hostName() (and others) with some
// kind of property?

class MonitorMainWindowModel : public QObject, public pera_software::aidkit::qt::Persistable {
	Q_OBJECT
	public:
		MonitorMainWindowModel();

		void readSettings( QSettings *settings ) noexcept override;
		void writeSettings( QSettings *settings ) const noexcept override;

		QString hostName() const;
		pera_software::aidkit::qt::net::Port portNumber() const;
		QString phoneBookPath() const;

	signals:
		void hostNameChanged( const QString &hostName );
		void portNumberChanged( pera_software::aidkit::qt::net::Port portNumber );
		void phoneBookPathChanged( const QString &phoneBookPath );

		void errorOccured( const QString &message );
		void showInformation( const QString &message );

		void showNotification( const QString &title, const QString &message );

	public slots:
		void quit();
		void browseForPhoneBook();

		void setHostName( const QString &hostName );
		void setPortNumber( pera_software::aidkit::qt::net::Port portNumber );
		void setPhoneBookPath( const QString &phoneBookPath );

	private:
		void connectToFritzBox();
		void onPhoneRinging( const QString &caller, const QString &callee );

		FritzBox *fritzBox_ = nullptr;
		FritzBoxPhoneBook fritzBoxPhoneBook_;
		QString hostName_;
		pera_software::aidkit::qt::net::Port portNumber_ = 0;
		QString phoneBookPath_;
};
