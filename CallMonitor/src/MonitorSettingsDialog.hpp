#pragma once

#include "MonitorSettingsDialogModel.hpp"
#include <pera_software/aidkit/qt/widgets/ForwardDeclarations.hpp>
#include <QDialog>

class QGroupBox;
class QPushButton;
class QLineEdit;

class MonitorSettingsDialog : public QDialog {
	Q_OBJECT
	public:
		MonitorSettingsDialog(QSharedPointer<MonitorSettings> settings, QWidget *parent = nullptr);
		~MonitorSettingsDialog() override;

		void setModel(QSharedPointer<MonitorSettingsDialogModel> model);

	Q_SIGNALS:

	public Q_SLOTS:

	private:
		QPushButton *okButton_;
		QLineEdit *hostNameWidget_;
		pera_software::aidkit::qt::IntegerSpinBox *portNumberWidget_;
		QLineEdit *phoneBookPathWidget_;
		QPushButton *browsePhoneBookPathButton_;

		pera_software::aidkit::qt::IntegerSpinBox *notificationTimeoutWidget_;

		QGroupBox *createFritzBoxWidgets();
		QGroupBox *createNotificationWidgets();

		QSharedPointer<MonitorSettings> settings_;
		QSharedPointer<MonitorSettingsDialogModel> model_;

		void browseForPhoneBook(QDir *lastVisitedDirectory, QString *selectedPhoneBookPath);
};
