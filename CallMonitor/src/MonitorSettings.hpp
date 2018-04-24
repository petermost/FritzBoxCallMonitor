#pragma once

#include "FritzBox.hpp"
#include <pera_software/aidkit/qt/core/Socket.hpp>
#include <pera_software/aidkit/qt/core/Persistable.hpp>
#include <QString>
#include <chrono>

class QSettings;

struct MonitorSettings : public pera_software::aidkit::qt::Persistable {
	MonitorSettings();

	virtual void readSettings(QSettings *settings) noexcept override;
	virtual void writeSettings(QSettings *settings) const noexcept override;

	QString phoneBookPath;
	QString hostName;
	std::chrono::milliseconds notificationTimeout;
	pera_software::aidkit::qt::Port portNumber;
};
