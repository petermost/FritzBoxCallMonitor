#pragma once

#include "MonitorSettings.hpp"
#include "MillisecondsProperty.hpp"
#include <QDir>
#include <QObject>
#include <pera_software/aidkit/qt/core/Socket.hpp>
#include <pera_software/aidkit/qt/properties/StringProperty.hpp>
#include <pera_software/aidkit/qt/properties/IntegerProperty.hpp>
#include <pera_software/aidkit/qt/properties/BooleanProperty.hpp>

class QSettings;

class MonitorSettingsDialogModel : public QObject {
	Q_OBJECT
	public:
		pera_software::aidkit::qt::StringProperty hostName;
		pera_software::aidkit::qt::BooleanProperty hostNameValid;

		pera_software::aidkit::qt::IntegerProperty portNumber;
		pera_software::aidkit::qt::StringProperty phoneBookPath;
		MillisecondsProperty notificationTimeout;
		pera_software::aidkit::qt::BooleanProperty okButton;

		MonitorSettingsDialogModel(QSharedPointer<MonitorSettings> settings, QObject *parent = nullptr);
		~MonitorSettingsDialogModel() override;

		void setData(const MonitorData &data);
		MonitorData data() const;

		void setLastVisitedDirectory(const QDir &directory);
		QDir lastVisitedDirectory() const;

	public Q_SLOTS:

	Q_SIGNALS:

	private:
		QSharedPointer<MonitorSettings> settings_;
		QDir lastVisitedDirectory_;
};
