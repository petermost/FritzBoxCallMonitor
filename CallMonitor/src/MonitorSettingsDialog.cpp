#include "MonitorSettingsDialog.hpp"
#include "MonitorSettingsDialogModel.hpp"

#include <pera_software/aidkit/qt/widgets/IntegerSpinBox.hpp>
#include <pera_software/aidkit/qt/properties/Bindings.hpp>
#include <pera_software/aidkit/qt/properties/BooleanProperty.hpp>
#include <pera_software/aidkit/qt/core/Strings.hpp>

#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFormLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSettings>

using namespace std;
using namespace chrono;
using namespace pera_software::aidkit::qt;

MonitorSettingsDialog::MonitorSettingsDialog(QSharedPointer<MonitorSettings> settings, QWidget *parent)
	: QDialog(parent), settings_(settings)
{
	auto buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	okButton_ = buttons->button(QDialogButtonBox::StandardButton::Ok);

	connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

	auto layout = new QVBoxLayout;
	layout->addWidget(createFritzBoxWidgets());
	layout->addWidget(createNotificationWidgets());
	layout->addWidget(buttons);
	setLayout(layout);

	settings_->readSize(this);
}

MonitorSettingsDialog::~MonitorSettingsDialog()
{
	settings_->writeSize(this);
}

QGroupBox *MonitorSettingsDialog::createFritzBoxWidgets()
{
	// Hostname widget:

	hostNameWidget_ = new QLineEdit;
	hostNameWidget_->setClearButtonEnabled(true);
	hostNameWidget_->setPlaceholderText(tr("Enter hostname (default fritz.box)"));

	// Hostname label:

	auto hostNameLabel = new QLabel(tr("&Hostname:"));
	hostNameLabel->setBuddy(hostNameWidget_);

	// Portnumber widget:

	portNumberWidget_ = new IntegerSpinBox;
	portNumberWidget_->setRange(PORT_MIN, PORT_MAX);

	// Portnumber label:

	auto portNumberLabel = new QLabel(tr("&Portnumber:"));
	portNumberLabel->setBuddy(portNumberWidget_);

	// Phonebook widget:

	phoneBookPathWidget_ = new QLineEdit;
	phoneBookPathWidget_->setClearButtonEnabled(true);
	phoneBookPathWidget_->setPlaceholderText(tr("Enter exported phonebook filename"));

	// Phonebook label:

	auto phoneBookLabel = new QLabel(tr("&Phonebook:"));
	phoneBookLabel->setBuddy(phoneBookPathWidget_);

	// Browse phonebook widget:

	browsePhoneBookPathButton_ = new QPushButton(tr("&Browse..."));

	// FritzBox widgets:

	auto fritzBoxLayout = new QGridLayout;
	fritzBoxLayout->addWidget(hostNameLabel, 0, 0);
	fritzBoxLayout->addWidget(hostNameWidget_, 0, 1);
	fritzBoxLayout->addWidget(portNumberLabel, 0, 2);
	fritzBoxLayout->addWidget(portNumberWidget_, 0, 3);

	fritzBoxLayout->addWidget(phoneBookLabel, 1, 0);
	fritzBoxLayout->addWidget(phoneBookPathWidget_, 1, 1);
	fritzBoxLayout->addWidget(browsePhoneBookPathButton_, 1, 2, 1, 2);

	auto fritzBoxGroup = new QGroupBox(tr("FRITZ!Box"));
	fritzBoxGroup->setLayout(fritzBoxLayout);

	return fritzBoxGroup;
}

QGroupBox *MonitorSettingsDialog::createNotificationWidgets()
{
	// Timeout widget:

	notificationTimeoutWidget_ = new IntegerSpinBox;
	notificationTimeoutWidget_->setSuffix(tr("ms"));
	auto currentPolicy = notificationTimeoutWidget_->sizePolicy();
	notificationTimeoutWidget_->setSizePolicy(QSizePolicy::Expanding, currentPolicy.verticalPolicy());

	// Timeout label:

	auto notificationTimeoutLabel = new QLabel(tr("&Timeout:"));
	notificationTimeoutLabel->setBuddy(notificationTimeoutWidget_);
	auto notificationLayout = new QHBoxLayout;
	notificationLayout->addWidget(notificationTimeoutLabel);
	notificationLayout->addWidget(notificationTimeoutWidget_);

	auto notificationGroup = new QGroupBox(tr("Notification"));
	notificationGroup->setLayout(notificationLayout);

	return notificationGroup;
}


void MonitorSettingsDialog::setModel(QSharedPointer<MonitorSettingsDialogModel> model)
{
	model_ = std::move(model);

	bindWidgetEnabledProperty(okButton_, &model_->okButton);
	bindLineEditTextProperty(hostNameWidget_, &model_->hostName);
	bindSpinBoxValueProperty(portNumberWidget_, &model_->portNumber);
	bindLineEditTextProperty(phoneBookPathWidget_, &model_->phoneBookPath);
	bindSpinBoxValueProperty(notificationTimeoutWidget_, &model_->notificationTimeout);

	bindWidgetEnabledProperty(browsePhoneBookPathButton_, &model_->browseForPhoneBookAction);
	bindWidgetClickedSignal(browsePhoneBookPathButton_, &model_->browseForPhoneBookAction);
	connect(model_.get(), &MonitorSettingsDialogModel::browseForPhoneBook, this, &MonitorSettingsDialog::browseForPhoneBook);
}

void MonitorSettingsDialog::browseForPhoneBook(QDir *lastVisitedDirectory, QString *selectedPhoneBookPath)
{
	QFileDialog fileDialog(this);
	fileDialog.setAcceptMode(QFileDialog::AcceptMode::AcceptOpen);
	fileDialog.setWindowTitle(tr("Select exported phone book"));
	fileDialog.setFileMode(QFileDialog::FileMode::ExistingFile);
	fileDialog.setViewMode(QFileDialog::ViewMode::Detail);
	fileDialog.setNameFilter(tr("Phonebooks (*.xml)"));
	fileDialog.setOption(QFileDialog::Option::DontUseNativeDialog);
	fileDialog.setDirectory(*lastVisitedDirectory);

	if (fileDialog.exec() == QFileDialog::Accepted) {
		QStringList phoneBookPaths = fileDialog.selectedFiles();
		Q_ASSERT(phoneBookPaths.size() >= 1);
		*selectedPhoneBookPath = phoneBookPaths[0];
		*lastVisitedDirectory = fileDialog.directory();
	}

}
