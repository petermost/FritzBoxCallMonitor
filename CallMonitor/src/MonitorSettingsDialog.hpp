#pragma once

#include "MonitorSettingsDialogModel.hpp"
#include <QDialog>

class QGroupBox;

class MonitorSettingsDialog : public QDialog {
	Q_OBJECT
	public:
		MonitorSettingsDialog(QSharedPointer<MonitorSettingsStorage> settingsStorage, QWidget *parent = nullptr);
		~MonitorSettingsDialog() override;

		void setSettings(const MonitorSettings &settings);
		MonitorSettings settings() const;

	Q_SIGNALS:

	public Q_SLOTS:
		void browseForPhoneBook();

	private:
		QGroupBox *createFritzBoxWidgets();
		QGroupBox *createNotificationWidgets();

		QSharedPointer<MonitorSettingsStorage> settingsStorage_;
		MonitorSettingsDialogModel model_;
};
