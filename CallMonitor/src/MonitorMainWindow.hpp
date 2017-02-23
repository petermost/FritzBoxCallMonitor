#pragma once

#include <pera_software/company/qt/PERAMainWindow.hpp>
#include <pera_software/aidkit/qt/widgets/MessagesWidget.hpp>
#include "FritzBox.hpp"
#include <QSystemTrayIcon>

class QSpinBox;
class QLineEdit;
class QPushButton;

class MonitorMainWindow : public pera_software::company::qt::PERAMainWindow {
	Q_OBJECT

	public:
		MonitorMainWindow();

	public slots:
		void connectToFritzBox();
		void disconnectFromFritzBox();

	protected:
		void closeEvent( QCloseEvent *event ) override;

	private slots:
		void onTrayIconActivated( QSystemTrayIcon::ActivationReason reason );

	private:
		FritzBox *fritzBox_ = nullptr;

		QLineEdit *hostName_;
		QSpinBox *portNumber_;

		QLineEdit *phoneBookName_;

		pera_software::aidkit::qt::MessagesWidget *messages_;
		QSystemTrayIcon *trayIcon_;
};
