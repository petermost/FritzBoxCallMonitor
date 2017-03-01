#pragma once

#include <pera_software/aidkit/qt/test/Test.hpp>

class FritzBoxPhoneBookTest : public pera_software::aidkit::qt::Test {
	Q_OBJECT

	private slots:
		void testRead();
};
