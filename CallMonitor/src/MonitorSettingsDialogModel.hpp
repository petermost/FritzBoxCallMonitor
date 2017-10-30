#pragma once

#include <pera_software/aidkit/qt/core/Socket.hpp>
#include <pera_software/aidkit/qt/core/Persistable.hpp>
#include <QDir>
#include <QObject>

class QSettings;

class MonitorSettingsDialogModel : public QObject, pera_software::aidkit::qt::Persistable {
	Q_OBJECT

	public:
		explicit MonitorSettingsDialogModel(QObject *parent = nullptr);
		~MonitorSettingsDialogModel();

		void readSettings(QSettings *settings) noexcept override;
		void writeSettings(QSettings *settings) const noexcept override;

		QString hostName() const;
		pera_software::aidkit::qt::Port portNumber() const;
		std::chrono::milliseconds notificationTimeout() const;
		QString phoneBookPath() const;
		QDir lastVisitedDirectory() const;

	signals:
		void hostNameChanged( const QString &hostName );
		void portNumberChanged( pera_software::aidkit::qt::Port portNumber );
		void notificationTimeoutChanged( std::chrono::milliseconds timeout );
		void phoneBookPathChanged( const QString &phoneBookPath );

	public slots:
		void setHostName( const QString &hostName );
		void setPortNumber( pera_software::aidkit::qt::Port portNumber );
		void setNotificationTimeout( std::chrono::milliseconds timeout );
		void setPhoneBookPath( const QString &phoneBookPath );
		void setLastVisitedDirectory( const QDir &directory );

	private:
		QString hostName_;
		pera_software::aidkit::qt::Port portNumber_ = 0;
		std::chrono::milliseconds notificationTimeout_;
		QString phoneBookPath_;
		QDir lastVisitedDirectory_;
};
