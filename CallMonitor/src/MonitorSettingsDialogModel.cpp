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

	auto updateOkButton = [=, this]() {
		hostNameValid = !hostName.value().isEmpty();
		okButton = hostNameValid;
	};

	connect(&hostName, &StringProperty::valueChanged, updateOkButton);
	connect(&portNumber, &IntegerProperty::valueChanged, updateOkButton);

	connect(&browseForPhoneBookAction, &QAction::triggered, this, &MonitorSettingsDialogModel::onBrowseForPhoneBook);
}

//==================================================================================================

MonitorSettingsDialogModel::~MonitorSettingsDialogModel()
{
	settings_->writeLastVisitedDirectory(lastVisitedDirectory_);
}

//==================================================================================================

void MonitorSettingsDialogModel::onBrowseForPhoneBook()
{
	QString selectedPhoneBookPath;
	Q_EMIT browseForPhoneBook(&lastVisitedDirectory_, &selectedPhoneBookPath);
	if (!selectedPhoneBookPath.isEmpty())
		phoneBookPath = selectedPhoneBookPath;
}
