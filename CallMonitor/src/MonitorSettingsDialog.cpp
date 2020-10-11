#include "MonitorSettingsDialog.hpp"
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFormLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSettings>
#include <pera_software/aidkit/qt/widgets/IntegerSpinBox.hpp>
#include <pera_software/aidkit/qt/properties/Bindings.hpp>
#include <pera_software/aidkit/qt/properties/BooleanProperty.hpp>
#include <pera_software/aidkit/qt/core/Strings.hpp>

using namespace std;
using namespace chrono;
using namespace pera_software::aidkit::qt;

MonitorSettingsDialog::MonitorSettingsDialog(QSharedPointer<MonitorSettings> settings, QWidget *parent)
	: QDialog(parent), settings_(settings), model_(settings)
{
	auto buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

	auto okButton = buttons->button(QDialogButtonBox::StandardButton::Ok);
	bindWidgetEnabledProperty(okButton, &model_.okButton);

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

	auto hostNameWidget = new QLineEdit;
	hostNameWidget->setClearButtonEnabled(true);
	hostNameWidget->setPlaceholderText(tr("Enter hostname (default fritz.box)"));
	bindLineEditTextProperty(hostNameWidget, &model_.hostName);

	// Hostname label:

	auto hostNameLabel = new QLabel(tr("&Hostname:"));
	hostNameLabel->setBuddy(hostNameWidget);

	// Portnumber widget:

	auto portNumberWidget = new IntegerSpinBox;
	portNumberWidget->setRange(PORT_MIN, PORT_MAX);
	bindSpinBoxValueProperty(portNumberWidget, &model_.portNumber);

	// Portnumber label:

	auto portNumberLabel = new QLabel(tr("&Portnumber:"));
	portNumberLabel->setBuddy(portNumberWidget);

	// Phonebook widget:

	auto phoneBookPathWidget = new QLineEdit;
	phoneBookPathWidget->setClearButtonEnabled(true);
	phoneBookPathWidget->setPlaceholderText(tr("Enter exported phonebook filename"));
	bindLineEditTextProperty(phoneBookPathWidget, &model_.phoneBookPath);

	// Phonebook label:

	auto phoneBookLabel = new QLabel(tr("&Phonebook:"));
	phoneBookLabel->setBuddy(phoneBookPathWidget);

	// Browse phonebook widget:

	auto browsePhoneBookPathButton = new QPushButton(tr("&Browse..."));
	connect(browsePhoneBookPathButton, &QPushButton::clicked, this, &MonitorSettingsDialog::browseForPhoneBook);

	// FritzBox widgets:

	auto fritzBoxLayout = new QGridLayout;
	fritzBoxLayout->addWidget(hostNameLabel, 0, 0);
	fritzBoxLayout->addWidget(hostNameWidget, 0, 1);
	fritzBoxLayout->addWidget(portNumberLabel, 0, 2);
	fritzBoxLayout->addWidget(portNumberWidget, 0, 3);

	fritzBoxLayout->addWidget(phoneBookLabel, 1, 0);
	fritzBoxLayout->addWidget(phoneBookPathWidget, 1, 1);
	fritzBoxLayout->addWidget(browsePhoneBookPathButton, 1, 2, 1, 2);

	auto fritzBoxGroup = new QGroupBox(tr("FRITZ!Box"));
	fritzBoxGroup->setLayout(fritzBoxLayout);

	return fritzBoxGroup;
}

QGroupBox *MonitorSettingsDialog::createNotificationWidgets()
{
	// Timeout widget:

	auto notificationTimeoutWidget = new IntegerSpinBox;
	notificationTimeoutWidget->setSuffix(tr("ms"));
	auto currentPolicy = notificationTimeoutWidget->sizePolicy();
	notificationTimeoutWidget->setSizePolicy(QSizePolicy::Expanding, currentPolicy.verticalPolicy());
	bindSpinBoxValueProperty(notificationTimeoutWidget, &model_.notificationTimeout);

	// Timeout label:

	auto notificationTimeoutLabel = new QLabel(tr("&Timeout:"));
	notificationTimeoutLabel->setBuddy(notificationTimeoutWidget);
	auto notificationLayout = new QHBoxLayout;
	notificationLayout->addWidget(notificationTimeoutLabel);
	notificationLayout->addWidget(notificationTimeoutWidget);

	auto notificationGroup = new QGroupBox(tr("Notification"));
	notificationGroup->setLayout(notificationLayout);

	return notificationGroup;
}

void MonitorSettingsDialog::setData(const MonitorData &data)
{
	model_.setData(data);
}

MonitorData MonitorSettingsDialog::data() const
{
	return model_.data();
}


void MonitorSettingsDialog::browseForPhoneBook()
{
	QFileDialog fileDialog(this);
	fileDialog.setAcceptMode(QFileDialog::AcceptMode::AcceptOpen);
	fileDialog.setWindowTitle(tr("Select exported phone book"));
	fileDialog.setFileMode(QFileDialog::FileMode::ExistingFile);
	fileDialog.setViewMode(QFileDialog::ViewMode::Detail);
	fileDialog.setNameFilter(tr("Phonebooks (*.xml)"));
	fileDialog.setOption(QFileDialog::Option::DontUseNativeDialog);
	fileDialog.setDirectory(model_.lastVisitedDirectory());

	if (fileDialog.exec() == QFileDialog::Accepted) {
		QStringList phoneBookPaths = fileDialog.selectedFiles();
		Q_ASSERT(phoneBookPaths.size() >= 1);
		model_.phoneBookPath = phoneBookPaths[0];
		model_.setLastVisitedDirectory(fileDialog.directory());
	}
}
