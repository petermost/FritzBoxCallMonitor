#pragma once

#include <QDialog>
#include "MonitorSettingsDialogModel.hpp"
#include <pera_software/aidkit/qt/core/Persistable.hpp>
#include <pera_software/aidkit/qt/widgets/ForwardDeclarations.hpp>

class QSpinBox;
class QLineEdit;
class QPushButton;
class QListView;
class QDialogButtonBox;

class MonitorSettingsDialog : public QDialog, public pera_software::aidkit::qt::Persistable {
	Q_OBJECT
	public:
		explicit MonitorSettingsDialog(QWidget *parent = nullptr);
		~MonitorSettingsDialog() override;

		void readSettings(QSettings *settings) noexcept override;
		void writeSettings(QSettings *settings) const noexcept override;

		MonitorSettingsDialogModel *model() {
			return &model_;
		}

	signals:

	public slots:
		void browseForPhoneBook();

	private:
		MonitorSettingsDialogModel model_;

		QLineEdit *hostName_;
		pera_software::aidkit::qt::IntegerSpinBox *portNumber_;
		QLineEdit *phoneBookPath_;
		QPushButton *browsePhoneBookPathButton_;
		pera_software::aidkit::qt::IntegerSpinBox *notificationTimeout_;
		QDialogButtonBox *buttons_;
};
