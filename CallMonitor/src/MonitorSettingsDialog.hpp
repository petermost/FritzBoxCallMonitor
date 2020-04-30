#pragma once

#include <QDialog>
#include "MonitorSettingsDialogModel.hpp"
#include <pera_software/aidkit/qt/core/Persistable.hpp>

class QGroupBox;

class MonitorSettingsDialog : public QDialog, public pera_software::aidkit::qt::Persistable {
	Q_OBJECT
	public:
		explicit MonitorSettingsDialog(QWidget *parent = nullptr);
		~MonitorSettingsDialog() override;

		void setSettings(const MonitorSettings &settings);
		MonitorSettings settings() const;

		void readSettings(QSettings *settings) noexcept override;
		void writeSettings(QSettings *settings) const noexcept override;

	signals:

	public slots:
		void browseForPhoneBook();

	private:
		QGroupBox *createFritzBoxWidgets();
		QGroupBox *createNotificationWidgets();

		MonitorSettingsDialogModel model_;
};
