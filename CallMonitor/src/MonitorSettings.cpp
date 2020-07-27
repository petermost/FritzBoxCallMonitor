#include "MonitorSettings.hpp"
#include "FritzBox.hpp"
#include <QSettings>
#include <pera_software/aidkit/qt/core/Chrono.hpp>

using namespace std::chrono;
using namespace pera_software::aidkit::qt;

static const QString HOST_NAME_KEY(QStringLiteral("hostName"));
static const QString PORT_NUMBER_KEY(QStringLiteral("portNumber"));
static const QString PHONE_BOOK_PATH(QStringLiteral("phoneBookPath"));
static const QString NOTIFICATION_TIMEOUT_KEY(QStringLiteral("notificationTimeout"));

static const milliseconds DEFAULT_NOTIFICATION_TIMEOUT(seconds(10));

MonitorSettings::MonitorSettings()
{
	hostName = FritzBox::DEFAULT_HOST_NAME;
	portNumber = FritzBox::DEFAULT_CALL_MONITOR_PORT;
	notificationTimeout = DEFAULT_NOTIFICATION_TIMEOUT;
}

void MonitorSettings::readSettings(QSettings *settings) noexcept
{
	hostName = qvariant_cast<QString>(settings->value(HOST_NAME_KEY, FritzBox::DEFAULT_HOST_NAME));
	portNumber = qvariant_cast<FritzBox::Port>(settings->value(PORT_NUMBER_KEY, FritzBox::DEFAULT_CALL_MONITOR_PORT));
	phoneBookPath = qvariant_cast<QString>(settings->value(PHONE_BOOK_PATH));
	notificationTimeout = qvariant_cast<milliseconds>(settings->value(NOTIFICATION_TIMEOUT_KEY, QVariant::fromValue(DEFAULT_NOTIFICATION_TIMEOUT)));
}

void MonitorSettings::writeSettings(QSettings *settings) const noexcept
{
	settings->setValue(HOST_NAME_KEY, hostName);
	settings->setValue(PORT_NUMBER_KEY, portNumber);
	settings->setValue(PHONE_BOOK_PATH, phoneBookPath);
	settings->setValue(NOTIFICATION_TIMEOUT_KEY, QVariant::fromValue(notificationTimeout));
}
