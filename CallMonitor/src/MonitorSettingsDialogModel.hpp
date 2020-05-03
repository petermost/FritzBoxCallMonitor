#pragma once

#include "MonitorSettings.hpp"
#include <QDir>
#include <QObject>
#include <pera_software/aidkit/qt/core/Persistable.hpp>
#include <pera_software/aidkit/qt/core/Socket.hpp>

class QSettings;

class MonitorSettingsDialogModel : public QObject, pera_software::aidkit::qt::Persistable {
	Q_OBJECT
	public:
		explicit MonitorSettingsDialogModel(QObject *parent = nullptr);
		~MonitorSettingsDialogModel() override;

		void setSettings(const MonitorSettings &settings);
		MonitorSettings settings() const;

		void readSettings(QSettings *settings) noexcept override;
		void writeSettings(QSettings *settings) const noexcept override;

		QDir lastVisitedDirectory() const;

	public slots:
		void setHostName(const QString &hostName);
		void setPortNumber(pera_software::aidkit::qt::Port portNumber);
		void setNotificationTimeout(std::chrono::milliseconds timeout);
		void setPhoneBookPath(const QString &phoneBookPath);
		void setLastVisitedDirectory(const QDir &directory);

	signals:
		void hostNameChanged(const QString &hostName);
		void portNumberChanged(pera_software::aidkit::qt::Port portNumber);
		void notificationTimeoutChanged(std::chrono::milliseconds timeout);
		void phoneBookPathChanged(const QString &phoneBookPath);

	private:
		MonitorSettings settings_;
		QDir lastVisitedDirectory_;
};
