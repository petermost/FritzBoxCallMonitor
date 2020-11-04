#pragma once

#include "FritzBox.hpp"
#include <QString>
#include <chrono>
#include <pera_software/aidkit/qt/core/PERASettings.hpp>
#include <pera_software/aidkit/qt/core/Socket.hpp>

class QDir;

class MonitorSettings : public pera_software::aidkit::qt::PERASettings {
	public:
		MonitorSettings();

		bool readVisibility();
		void writeVisibility(bool isVisible);

		QDir readLastVisitedDirectory();
		void writeLastVisitedDirectory(const QDir &lastVisitedDirectory);

		QString readHostName();
		void writeHostName(const QString &hostName);

		pera_software::aidkit::qt::Port readPortNumber();
		void writePortNumber(pera_software::aidkit::qt::Port portNumber);

		QString readPhoneBookPath();
		void writePhoneBookPath(const QString phoneBookPath);

		std::chrono::milliseconds readNotificationTimeout();
		void writeNotificationTimeout(std::chrono::milliseconds notificationTimeout);
};
