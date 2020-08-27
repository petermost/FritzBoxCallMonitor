#pragma once

#include "MonitorSettings.hpp"
#include <QDir>
#include <QObject>
#include <pera_software/aidkit/qt/core/Socket.hpp>

class QSettings;

class MonitorSettingsDialogModel : public QObject {
	Q_OBJECT
	public:
		MonitorSettingsDialogModel(QSharedPointer<MonitorSettings> settings, QObject *parent = nullptr);
		~MonitorSettingsDialogModel() override;

		void setData(const MonitorData &data);
		MonitorData data() const;

		QDir lastVisitedDirectory() const;

	public Q_SLOTS:
		void setHostName(const QString &hostName);
		void setPortNumber(pera_software::aidkit::qt::Port portNumber);
		void setNotificationTimeout(std::chrono::milliseconds notificationTimeout);
		void setPhoneBookPath(const QString &phoneBookPath);
		void setLastVisitedDirectory(const QDir &directory);

	Q_SIGNALS:
		void hostNameChanged(const QString &hostName);
		void portNumberChanged(pera_software::aidkit::qt::Port portNumber);
		void notificationTimeoutChanged(std::chrono::milliseconds timeout);
		void phoneBookPathChanged(const QString &phoneBookPath);

	private:
		QSharedPointer<MonitorSettings> settings_;
		MonitorData data_;
		QDir lastVisitedDirectory_;
};
