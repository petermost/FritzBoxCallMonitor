#pragma once

#include "MonitorTrayIcon.hpp"
#include "MonitorMainWindowModel.hpp"
#include <pera_software/aidkit/qt/widgets/ForwardDeclarations.hpp>
#include <pera_software/aidkit/qt/widgets/PERAMainWindow.hpp>

class MonitorMainWindow : public pera_software::aidkit::qt::PERAMainWindow {
	Q_OBJECT
	public:
		MonitorMainWindow(QSharedPointer<MonitorSettings> settings);

		QAction *showAction();
		QAction *hideAction();

	public Q_SLOTS:
		void onQuit();
		void onAbout();

	private Q_SLOTS:
		void onEditSettings();
		void onVisible(bool isVisible);

	private:
		void addEditMenu();
		void addHelpMenu();
		void addWindowMenu();
		void addStatusBar();
		void addTrayIcon();

		QSharedPointer<MonitorSettings> settings_;
		MonitorMainWindowModel model_;

		QAction *showAction_ = nullptr;
		QAction *hideAction_ = nullptr;

		pera_software::aidkit::qt::MessagesView *messages_;
		MonitorTrayIcon *trayIcon_;
};
