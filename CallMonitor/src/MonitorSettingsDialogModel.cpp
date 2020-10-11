#include "MonitorSettingsDialogModel.hpp"
#include "FritzBox.hpp"
#include "MonitorSettings.hpp"
#include <QSettings>
#include <QStandardPaths>
#include <pera_software/aidkit/qt/core/Chrono.hpp>

using namespace std::chrono;
using namespace pera_software::aidkit::qt;

//==================================================================================================

MonitorSettingsDialogModel::MonitorSettingsDialogModel(QSharedPointer<MonitorSettings> settings, QObject *parent)
	: QObject(parent)
	, hostNameValid(true)
	, okButton(true)
	, settings_(settings)
{
	lastVisitedDirectory_ = settings_->readLastVisitedDirectory();

	auto enableOkButton = [=, this]() {
		hostNameValid = !hostName.value().isEmpty();
		okButton = hostNameValid;
	};

	connect(&hostName, &StringProperty::valueChanged, enableOkButton);
	connect(&portNumber, &IntegerProperty::valueChanged, enableOkButton);
}

//==================================================================================================

MonitorSettingsDialogModel::~MonitorSettingsDialogModel()
{
	settings_->writeLastVisitedDirectory(lastVisitedDirectory_);
}

//==================================================================================================

void MonitorSettingsDialogModel::setData(const MonitorData &data)
{
	hostName = data.hostName;
	portNumber = data.portNumber;
	phoneBookPath = data.phoneBookPath;
	notificationTimeout = data.notificationTimeout;
}

//==================================================================================================

MonitorData MonitorSettingsDialogModel::data() const
{
	MonitorData data = {
		.hostName = hostName,
		.portNumber = static_cast<Port>(portNumber),
		.notificationTimeout = notificationTimeout,
		.phoneBookPath = phoneBookPath
	};
	return data;
}

//==================================================================================================

void MonitorSettingsDialogModel::setLastVisitedDirectory(const QDir &directory)
{
	lastVisitedDirectory_ = directory;
}

//==================================================================================================

QDir MonitorSettingsDialogModel::lastVisitedDirectory() const
{
	return lastVisitedDirectory_;
}
