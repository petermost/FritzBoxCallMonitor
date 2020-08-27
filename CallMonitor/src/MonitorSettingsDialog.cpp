#include "MonitorSettingsDialog.hpp"
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSettings>
#include <pera_software/aidkit/stdlib/stdlib.hpp>
#include <pera_software/aidkit/qt/widgets/IntegerSpinBox.hpp>

using namespace std;
using namespace chrono;
using namespace pera_software::aidkit::qt;
using namespace pera_software::aidkit::stdlib;

MonitorSettingsDialog::MonitorSettingsDialog(QSharedPointer<MonitorSettings> settings, QWidget *parent)
	: QDialog(parent), settings_(settings), model_(settings)
{
	auto buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
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

	auto hostName = new QLineEdit;
	hostName->setClearButtonEnabled(true);
	hostName->setText(model_.data().hostName);
	connect(hostName, &QLineEdit::editingFinished, [=, this] {
		model_.setHostName(hostName->text());
	});
	connect(&model_, &MonitorSettingsDialogModel::hostNameChanged, hostName, &QLineEdit::setText);

	// Hostname label:

	auto hostNameLabel = new QLabel(tr("&Hostname:"));
	hostNameLabel->setBuddy(hostName);

	// Portnumber widget:

	auto portNumber = new IntegerSpinBox;
	portNumber->setRange(PORT_MIN, PORT_MAX);
	portNumber->setValue(model_.data().portNumber);
	connect(portNumber, &IntegerSpinBox::editingFinished, [=, this] {
		model_.setPortNumber(static_cast<Port>(portNumber->value()));
	});
	connect(&model_, &MonitorSettingsDialogModel::portNumberChanged, portNumber, &IntegerSpinBox::setValue);

	// Portnumber label:

	auto portNumberLabel = new QLabel(tr("&Portnumber:"));
	portNumberLabel->setBuddy(portNumber);

	// Phonebook widget:

	auto phoneBookPath = new QLineEdit;
	phoneBookPath->setClearButtonEnabled(true);
	phoneBookPath->setText(model_.data().phoneBookPath);
	connect(phoneBookPath, &QLineEdit::editingFinished, &model_, [=, this] {
		model_.setPhoneBookPath(phoneBookPath->text());
	});
	connect(&model_, &MonitorSettingsDialogModel::phoneBookPathChanged, phoneBookPath, &QLineEdit::setText);

	// Phonebook label:

	auto phoneBookLabel = new QLabel(tr("&Phonebook:"));
	phoneBookLabel->setBuddy(phoneBookPath);

	// Browse phonebook widget:

	auto browsePhoneBookPathButton = new QPushButton(tr("&Browse..."));
	connect(browsePhoneBookPathButton, &QPushButton::clicked, this, &MonitorSettingsDialog::browseForPhoneBook);

	// FritzBox widgets:

	auto fritzBoxLayout = new QGridLayout;
	fritzBoxLayout->addWidget(hostNameLabel, 0, 0);
	fritzBoxLayout->addWidget(hostName, 0, 1);
	fritzBoxLayout->addWidget(portNumberLabel, 0, 2);
	fritzBoxLayout->addWidget(portNumber, 0, 3);

	fritzBoxLayout->addWidget(phoneBookLabel, 1, 0);
	fritzBoxLayout->addWidget(phoneBookPath, 1, 1);
	fritzBoxLayout->addWidget(browsePhoneBookPathButton, 1, 2, 1, 2);

	auto fritzBoxGroup = new QGroupBox(tr("FRITZ!Box"));
	fritzBoxGroup->setLayout(fritzBoxLayout);

	return fritzBoxGroup;
}

QGroupBox *MonitorSettingsDialog::createNotificationWidgets()
{
	// Timeout widget:

	auto notificationTimeout = new IntegerSpinBox;
	notificationTimeout->setSuffix(tr("ms"));
	notificationTimeout->setValue(int_cast<int>(model_.data().notificationTimeout.count()));
	connect(notificationTimeout, &IntegerSpinBox::editingFinished, [=, this] {
		model_.setNotificationTimeout(milliseconds(notificationTimeout->value()));
	});
	connect(&model_, &MonitorSettingsDialogModel::notificationTimeoutChanged, [=](milliseconds timeout) {
		notificationTimeout->setValue(int_cast<int>(timeout.count()));
	});

	// Timeout label:

	auto notificationTimeoutLabel = new QLabel(tr("&Timeout:"));
	notificationTimeoutLabel->setBuddy(notificationTimeout);

	auto notificationLayout = new QHBoxLayout;
	notificationLayout->addWidget(notificationTimeoutLabel);
	notificationLayout->addWidget(notificationTimeout);

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
		model_.setPhoneBookPath(phoneBookPaths[0]);
		model_.setLastVisitedDirectory(fileDialog.directory());
	}
}
