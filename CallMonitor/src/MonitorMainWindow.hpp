#pragma once

#include "MonitorMainWindowModel.hpp"
#include <pera_software/company/qt/PERAMainWindow.hpp>
#include <pera_software/aidkit/qt/widgets/MessagesWidget.hpp>
#include <QSystemTrayIcon>

class QSpinBox;
class QLineEdit;
class QPushButton;

class MonitorMainWindow : public pera_software::company::qt::PERAMainWindow {
	Q_OBJECT

	public:
		MonitorMainWindow();

		virtual void readSettings( QSettings *settings ) override;
		virtual void writeSettings( QSettings *settings ) const override;

	public slots:
		void quit();

	protected:
		void closeEvent( QCloseEvent *event ) override;

	private slots:
		void onTrayIconActivated( QSystemTrayIcon::ActivationReason reason );

	private:
		MonitorMainWindowModel model_;

		QLineEdit *hostName_;
		QSpinBox *portNumber_;
		QLineEdit *phoneBookPath_;
		QPushButton *browsePhoneBookPathButton_;
		pera_software::aidkit::qt::MessagesWidget *messages_;

		QSystemTrayIcon *trayIcon_;
};
