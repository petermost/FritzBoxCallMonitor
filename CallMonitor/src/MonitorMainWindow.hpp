#pragma once

#include "MonitorTrayIcon.hpp"
#include <pera_software/aidkit/qt/widgets/ForwardDeclarations.hpp>
#include <pera_software/aidkit/qt/widgets/MessagesView.hpp>
#include <pera_software/aidkit/qt/widgets/PERAMainWindow.hpp>

class MonitorSettings;
class MonitorMainWindowModel;

class MonitorMainWindow : public pera_software::aidkit::qt::PERAMainWindow {
	Q_OBJECT
	public:
		MonitorMainWindow(QSharedPointer<MonitorSettings> settings);

		QAction *showAction();
		QAction *hideAction();

		void setModel(QSharedPointer<MonitorMainWindowModel> model);

	public Q_SLOTS:
		void onQuit();
		void onAbout();
		void setVisible(bool visible) override;

	private Q_SLOTS:
		void onEditSettings();

	private:
		void addEditMenu();
		void addHelpMenu();
		void addWindowMenu();
		void addStatusBar();
		void addTrayIcon();

		QSharedPointer<MonitorSettings> settings_;
		QSharedPointer<MonitorMainWindowModel> model_;

		QAction *showAction_ = nullptr;
		QAction *hideAction_ = nullptr;

		pera_software::aidkit::qt::MessagesView messages_;
		MonitorTrayIcon trayIcon_;
};
