#pragma once

#include "MonitorSettingsDialogModel.hpp"
#include <QDialog>

class QGroupBox;

class MonitorSettingsDialog : public QDialog {
	Q_OBJECT
	public:
		MonitorSettingsDialog(QSharedPointer<MonitorSettings> settings, QWidget *parent = nullptr);
		~MonitorSettingsDialog() override;

		void setData(const MonitorData &data);
		MonitorData data() const;

	Q_SIGNALS:

	public Q_SLOTS:
		void browseForPhoneBook();

	private:
		QGroupBox *createFritzBoxWidgets();
		QGroupBox *createNotificationWidgets();

		QSharedPointer<MonitorSettings> settings_;
		MonitorSettingsDialogModel model_;
};
