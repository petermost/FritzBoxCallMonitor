#pragma once

#include "MonitorMainWindowModel.hpp"
#include <QSystemTrayIcon>
#include <pera_software/aidkit/qt/widgets/ForwardDeclarations.hpp>
#include <pera_software/company/qt/PERAMainWindow.hpp>

class MonitorMainWindow : public pera_software::company::qt::PERAMainWindow {
	Q_OBJECT
	public:
		MonitorMainWindow();
		~MonitorMainWindow() override;

		void readSettings(QSettings *settings) noexcept override;
		void writeSettings(QSettings *settings) const noexcept override;

	public Q_SLOTS:
		void onQuit();
		void onAbout();

	private Q_SLOTS:
		void onEditSettings();
		void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
		void onVisible(bool isVisible);

	private:
		void addEditMenu();
		void addHelpMenu();
		void addWindowMenu();
		void addStatusBar();
		void addTrayIcon();

		MonitorMainWindowModel model_;

		pera_software::aidkit::qt::MessagesView *messages_;

		QSystemTrayIcon *trayIcon_;
};
