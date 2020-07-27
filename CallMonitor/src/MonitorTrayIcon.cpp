// Copyright 2020 Peter Most, PERA Software Solutions GmbH
//
// This file is part of the CppAidKit library.
//
// CppAidKit is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// CppAidKit is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with CppAidKit. If not, see <http://www.gnu.org/licenses/>.

#include "MonitorTrayIcon.hpp"
#include "MonitorMainWindow.hpp"
#include <QMenu>
#include <pera_software/aidkit/qt/core/Strings.hpp>

using namespace pera_software::aidkit::qt;

MonitorTrayIcon::MonitorTrayIcon(MonitorMainWindow *mainWindow)
	: QSystemTrayIcon(mainWindow)
{
	// Prepare the menu:

	auto menu = new QMenu("TrayMenu"_qs, mainWindow);
	menu->addAction(mainWindow->showAction());
	menu->addAction(mainWindow->hideAction());
	menu->addSeparator();
	menu->addAction(mainWindow->quitAction());
	setContextMenu(menu);

	// Prepare the icon:

	QIcon fritzBoxIcon(":/telephone-icon.png"_qs);
	setIcon(fritzBoxIcon);

	show();
}
