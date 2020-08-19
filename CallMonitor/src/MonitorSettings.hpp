#pragma once

#include "FritzBox.hpp"
#include <QString>
#include <chrono>
#include <pera_software/aidkit/qt/core/PERASettingsStorage.hpp>
#include <pera_software/aidkit/qt/core/Socket.hpp>

class QDir;
class QSettings;

struct MonitorSettings {
	QString hostName;
	pera_software::aidkit::qt::Port portNumber = 0;
	std::chrono::milliseconds notificationTimeout;
	QString phoneBookPath;
};

class MonitorSettingsStorage : public pera_software::aidkit::qt::PERASettingsStorage {
	public:
		MonitorSettingsStorage();

		bool readVisibility();
		void writeVisibility(bool isVisible);

		QDir readLastVisitedDirectory();
		void writeLastVisitedDirectory(const QDir &lastVisitedDirectory);

		MonitorSettings readSettings();
		void writeSettings(const MonitorSettings &settings);
};
