#include <iostream>
#include <QApplication>
#include "SimulatorMainWindow.hpp"

using namespace std;

int main( int argc, char *argv[] ) {
	QApplication application( argc, argv );

	SimulatorMainWindow mainWindow;
	mainWindow.show();

	return application.exec();
}
