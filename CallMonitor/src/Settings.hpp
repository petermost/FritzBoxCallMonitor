// Copyright 2017 Peter Most, PERA Software Solutions GmbH
//
// This file is part of the CppAidKit library.
//
// CppAidKit is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// CppAidKit is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with CppAidKit. If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include "FritzBox.hpp"
#include <pera_software/aidkit/qt/core/Socket.hpp>
#include <QString>
#include <chrono>

class QSettings;

extern const std::chrono::milliseconds DEFAULT_NOTIFICATION_TIMEOUT;
extern const QString DEFAULT_HOST_NAME;
extern const pera_software::aidkit::qt::Port DEFAULT_CALL_MONITOR_PORT;

QString readHostName(QSettings *settings) noexcept;
void writeHostName(QSettings *settings, const QString &hostName) noexcept;

pera_software::aidkit::qt::Port readPortNumber(QSettings *settings) noexcept;
void writePortNumber(QSettings *settings, pera_software::aidkit::qt::Port portNumber) noexcept;

QString readPhoneBookPath(QSettings *settings) noexcept;
void writePhoneBookPath(QSettings *settings, const QString &phoneBookPath) noexcept;

std::chrono::milliseconds readNotificationTimeout(QSettings *settings) noexcept;
void writeNotificationTimeout(QSettings *settings, std::chrono::milliseconds notificationTimeout) noexcept;
