#pragma once

#include "MonitorMainWindowModel.hpp"
#include <pera_software/company/qt/PERAMainWindow.hpp>
#include <pera_software/aidkit/qt/widgets/ForwardDeclarations.hpp>
#include <QSystemTrayIcon>

class QSpinBox;
class QLineEdit;
class QPushButton;

class MonitorMainWindow : public pera_software::company::qt::PERAMainWindow {
	Q_OBJECT

	public:
		MonitorMainWindow();

		void readSettings( QSettings *settings ) noexcept override;
		void writeSettings( QSettings *settings ) const noexcept override;

	public slots:
		void quit();

	private slots:
		void onTrayIconActivated( QSystemTrayIcon::ActivationReason reason );
		void onHide();

	private:
		void addWindowMenu();
		void addTrayIcon();

		MonitorMainWindowModel model_;

		QLineEdit *hostName_;
		pera_software::aidkit::qt::IntegerSpinBox *portNumber_;
		QLineEdit *phoneBookPath_;
		QPushButton *browsePhoneBookPathButton_;
		pera_software::aidkit::qt::IntegerSpinBox *notificationTimeout_;
		pera_software::aidkit::qt::MessagesWidget *messages_;

		QSystemTrayIcon *trayIcon_;
};
