#pragma once

#include "MonitorTrayIcon.hpp"
#include "MonitorMainWindowModel.hpp"
#include <pera_software/aidkit/qt/widgets/ForwardDeclarations.hpp>
#include <pera_software/company/qt/PERAMainWindow.hpp>

class MonitorMainWindow : public pera_software::company::qt::PERAMainWindow {
	Q_OBJECT
	public:
		MonitorMainWindow();
		~MonitorMainWindow() override;

		QAction *showAction();
		QAction *hideAction();

		void readSettings(QSettings *settings) noexcept override;
		void writeSettings(QSettings *settings) const noexcept override;

	public Q_SLOTS:
		void onQuit();
		void onAbout();

	private Q_SLOTS:
		void onEditSettings();
		void onVisible(bool isVisible);

	private:
		QAction *showAction_ = nullptr;
		QAction *hideAction_ = nullptr;

		void addEditMenu();
		void addHelpMenu();
		void addWindowMenu();
		void addStatusBar();
		void addTrayIcon();

		MonitorMainWindowModel model_;
		pera_software::aidkit::qt::MessagesView *messages_;
		MonitorTrayIcon *trayIcon_;
};
