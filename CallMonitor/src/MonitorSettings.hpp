#pragma once

#include "FritzBox.hpp"
#include <QString>
#include <chrono>
#include <pera_software/aidkit/qt/core/PERASettings.hpp>
#include <pera_software/aidkit/qt/core/Socket.hpp>

class QDir;
class QSettings;

struct MonitorData {
	QString hostName;
	pera_software::aidkit::qt::Port portNumber = 0;
	std::chrono::milliseconds notificationTimeout;
	QString phoneBookPath;
};

class MonitorSettings : public pera_software::aidkit::qt::PERASettings {
	public:
		MonitorSettings();

		bool readVisibility();
		void writeVisibility(bool isVisible);

		QDir readLastVisitedDirectory();
		void writeLastVisitedDirectory(const QDir &lastVisitedDirectory);

		MonitorData readData();
		void writeData(const MonitorData &data);
};
