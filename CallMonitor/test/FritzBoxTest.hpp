#pragma once

#include <pera_software/aidkit/qt/test/Test.hpp>

class FritzBoxTest : public pera_software::aidkit::qt::Test {
	Q_OBJECT

	public:
		static const QByteArray RING_DATA;
		static const QByteArray CONNECT_DATA;
		static const QByteArray DISCONNECT_DATA;
		static const QByteArray CALLING_DATA;

	private slots:
		void testRingingSignal();
		void testConnectedSignal();
		void testDisconnectedSignal();
		void testCallingSignal();
};
