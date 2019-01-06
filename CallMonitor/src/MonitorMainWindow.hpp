#pragma once

#include "MonitorMainWindowModel.hpp"
#include <pera_software/company/qt/PERAMainWindow.hpp>
#include <pera_software/aidkit/qt/widgets/ForwardDeclarations.hpp>
#include <QSystemTrayIcon>

class MonitorMainWindow : public pera_software::company::qt::PERAMainWindow {
	Q_OBJECT

	public:
		MonitorMainWindow();
		~MonitorMainWindow() override;

		void readSettings(QSettings *settings) noexcept override;
		void writeSettings(QSettings *settings) const noexcept override;

	public slots:
		void quit();

	private slots:
		void editSettings();
		void onTrayIconActivated( QSystemTrayIcon::ActivationReason reason );
		void onVisible( bool isVisible );

	private:
		void addEditMenu();
		void addWindowMenu();
		void addStatusBar();
		void addTrayIcon();

		MonitorMainWindowModel model_;

		pera_software::aidkit::qt::MessagesView *messages_;

		QSystemTrayIcon *trayIcon_;
};
