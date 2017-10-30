#include "MonitorSettingsDialog.hpp"
#include <pera_software/aidkit/qt/widgets/IntegerSpinBox.hpp>
#include <pera_software/aidkit/qt/widgets/Widgets.hpp>
#include <pera_software/aidkit/cpp/stdlib.hpp>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QDialogButtonBox>
#include <QSettings>

using namespace std;
using namespace chrono;
using namespace pera_software::aidkit::qt;
using namespace pera_software::aidkit::cpp;

//==================================================================================================

MonitorSettingsDialog::MonitorSettingsDialog(QWidget *parent)
		: QDialog(parent) {

	// Prepare the hostname widget:

	hostName_ = new QLineEdit;
	hostName_->setClearButtonEnabled( true );
	hostName_->setText(model_.hostName());
	connect( hostName_, &QLineEdit::editingFinished, [ = ] {
		model_.setHostName( hostName_->text() );
	});
	connect( &model_, &MonitorSettingsDialogModel::hostNameChanged, hostName_, &QLineEdit::setText );

	auto hostNameLabel = new QLabel( tr( "&Hostname:" ));
	hostNameLabel->setBuddy( hostName_ );

	// Prepare the port number widget:

	portNumber_ = new IntegerSpinBox;
	portNumber_->setRange( PORT_MIN, PORT_MAX );
	portNumber_->setValue( model_.portNumber() );
	connect( portNumber_, &IntegerSpinBox::editingFinished, [ = ] {
		model_.setPortNumber( static_cast< Port >( portNumber_->value() ));
	});
	connect( &model_, &MonitorSettingsDialogModel::portNumberChanged, portNumber_, &IntegerSpinBox::setValue );


	auto portNumberLabel = new QLabel( tr( "&Portnumber:" ));
	portNumberLabel->setBuddy( portNumber_ );

	// Prepare the phone book widget:

	phoneBookPath_ = new QLineEdit;
	phoneBookPath_->setClearButtonEnabled( true );
	phoneBookPath_->setText( model_.phoneBookPath() );
	connect( phoneBookPath_, &QLineEdit::editingFinished, &model_, [ = ] {
		model_.setPhoneBookPath( phoneBookPath_->text() );
	});
	connect( &model_, &MonitorSettingsDialogModel::phoneBookPathChanged, phoneBookPath_, &QLineEdit::setText );

	auto phoneBookLabel = new QLabel( tr( "&Phonebook:" ));
	phoneBookLabel->setBuddy( phoneBookPath_ );

	// Prepare the phone book browse widget:

	browsePhoneBookPathButton_ = new QPushButton( tr( "&Browse..." ));
	connect( browsePhoneBookPathButton_, &QPushButton::clicked, this, &MonitorSettingsDialog::browseForPhoneBook );

	// Prepare the fritz box layout:

	auto fritzBoxLayout = new QGridLayout;
	fritzBoxLayout->addWidget( hostNameLabel, 0, 0 );
	fritzBoxLayout->addWidget( hostName_, 0, 1 );
	fritzBoxLayout->addWidget( portNumberLabel, 0, 2 );
	fritzBoxLayout->addWidget( portNumber_, 0, 3 );

	fritzBoxLayout->addWidget( phoneBookLabel, 1, 0 );
	fritzBoxLayout->addWidget( phoneBookPath_, 1, 1 );
	fritzBoxLayout->addWidget( browsePhoneBookPathButton_, 1, 2, 1, 2 );

	auto fritzBoxGroup = new QGroupBox( "FRITZ!Box" );
	fritzBoxGroup->setLayout( fritzBoxLayout );

	// Prepare the notification layout:

	notificationTimeout_ = new IntegerSpinBox;
	notificationTimeout_->setSuffix( tr( "ms" ));
	auto notificationTimeoutLabel = new QLabel( tr( "&Timeout:" ));
	notificationTimeoutLabel->setBuddy( notificationTimeout_ );
	notificationTimeout_->setValue( int_cast< int >( model_.notificationTimeout().count() ));
	connect( notificationTimeout_, &IntegerSpinBox::editingFinished, [ = ] {
		model_.setNotificationTimeout( milliseconds( notificationTimeout_->value() ));
	});
	connect( &model_, &MonitorSettingsDialogModel::notificationTimeoutChanged, [ = ]( milliseconds timeout ) {
		notificationTimeout_->setValue( int_cast< int >( timeout.count() ));
	});

	auto notificationLayout = new QHBoxLayout;
	notificationLayout->addWidget( notificationTimeoutLabel );
	notificationLayout->addWidget( notificationTimeout_ );

	auto notificationGroup = new QGroupBox( tr( "Notification" ));
	notificationGroup->setLayout( notificationLayout );

	buttons_ = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	connect(buttons_, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttons_, &QDialogButtonBox::rejected, this, &QDialog::reject);

	auto layout = new QVBoxLayout;
	layout->addWidget( fritzBoxGroup );
	layout->addWidget( notificationGroup );
	layout->addWidget(buttons_);
	setLayout(layout);

	QSettings settings;
	readSettings(&settings);
}

//==================================================================================================

MonitorSettingsDialog::~MonitorSettingsDialog() {
	QSettings settings;
	writeSettings(&settings);
}

//==================================================================================================

void MonitorSettingsDialog::readSettings(QSettings *settings) noexcept {
	model_.readSettings(settings);
	Widgets::readGeometry(this, settings);
}

//==================================================================================================

void MonitorSettingsDialog::writeSettings(QSettings *settings) const noexcept {
	model_.writeSettings(settings);
	Widgets::writeGeometry(this, settings);
}

//==================================================================================================

void MonitorSettingsDialog::browseForPhoneBook() {
	QFileDialog fileDialog( this );
	fileDialog.setAcceptMode( QFileDialog::AcceptMode::AcceptOpen );
	fileDialog.setWindowTitle( tr( "Select exported phone book"));
	fileDialog.setFileMode( QFileDialog::FileMode::ExistingFile );
	fileDialog.setViewMode( QFileDialog::ViewMode::Detail );
	fileDialog.setNameFilter( tr( "Phonebooks (*.xml)"));
	fileDialog.setOption( QFileDialog::Option::DontUseNativeDialog );
	fileDialog.setDirectory( model_.lastVisitedDirectory() );

	if ( fileDialog.exec() == QFileDialog::Accepted ) {
		QStringList phoneBookPaths = fileDialog.selectedFiles();
		Q_ASSERT( phoneBookPaths.size() >= 1 );
		model_.setPhoneBookPath( phoneBookPaths[ 0 ]);
		model_.setLastVisitedDirectory( fileDialog.directory() );
	}
}
