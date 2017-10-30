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

#include "Settings.hpp"
#include <QSettings>
#include "FritzBox.hpp"
#include <pera_software/aidkit/qt/core/Chrono.hpp>

using namespace std::chrono;
using namespace pera_software::aidkit::qt;

static const QString HOST_NAME_KEY( QStringLiteral( "hostName" ));
static const QString PORT_NUMBER_KEY( QStringLiteral( "portNumber" ));
static const QString PHONE_BOOK_PATH( QStringLiteral( "phoneBookPath" ));
static const QString NOTIFICATION_TIMEOUT_KEY( QStringLiteral( "notificationTimeout" ));

const milliseconds DEFAULT_NOTIFICATION_TIMEOUT( seconds( 10 ));
const QString DEFAULT_HOST_NAME( FritzBox::DEFAULT_HOST_NAME );
const Port DEFAULT_CALL_MONITOR_PORT( FritzBox::DEFAULT_CALL_MONITOR_PORT );


QString readHostName(QSettings *settings) noexcept {
	return qvariant_cast< QString >( settings->value( HOST_NAME_KEY, DEFAULT_HOST_NAME ));
}

void writeHostName(QSettings *settings, const QString &hostName) noexcept {
	settings->setValue( HOST_NAME_KEY, hostName );
}

FritzBox::Port readPortNumber(QSettings *settings) noexcept {
	return qvariant_cast< FritzBox::Port >( settings->value( PORT_NUMBER_KEY, DEFAULT_CALL_MONITOR_PORT ));
}

void writePortNumber(QSettings *settings, FritzBox::Port portNumber) noexcept {
	settings->setValue( PORT_NUMBER_KEY, portNumber );
}

QString readPhoneBookPath(QSettings *settings) noexcept {
	return qvariant_cast< QString >( settings->value( PHONE_BOOK_PATH ));
}

void writePhoneBookPath(QSettings *settings, const QString &phoneBookPath) noexcept {
	settings->setValue( PHONE_BOOK_PATH, phoneBookPath );
}

milliseconds readNotificationTimeout(QSettings *settings) noexcept {
	return qvariant_cast< milliseconds >( settings->value( NOTIFICATION_TIMEOUT_KEY, QVariant::fromValue( DEFAULT_NOTIFICATION_TIMEOUT )));
}

void writeNotificationTimeout(QSettings *settings, milliseconds notificationTimeout) noexcept {
	settings->setValue( NOTIFICATION_TIMEOUT_KEY, QVariant::fromValue( notificationTimeout ));
}
