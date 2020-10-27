#pragma once

#include "MonitorSettings.hpp"
#include "MillisecondsProperty.hpp"

#include <pera_software/aidkit/qt/core/Socket.hpp>
#include <pera_software/aidkit/qt/properties/StringProperty.hpp>
#include <pera_software/aidkit/qt/properties/IntegerProperty.hpp>
#include <pera_software/aidkit/qt/properties/BooleanProperty.hpp>

#include <QDir>
#include <QAction>
#include <QObject>
#include <QSharedPointer>

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

		QAction browseForPhoneBookAction;

		MonitorSettingsDialogModel(QSharedPointer<MonitorSettings> settings, QObject *parent = nullptr);
		~MonitorSettingsDialogModel() override;

	public Q_SLOTS:
		void onBrowseForPhoneBook();

	Q_SIGNALS:
		void browseForPhoneBook(QDir *lastVisitedDirectory, QString *selectedPhoneBookPath);

	private:
		QSharedPointer<MonitorSettings> settings_;
		QDir lastVisitedDirectory_;
};
