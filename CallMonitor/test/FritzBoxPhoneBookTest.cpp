#include "FritzBoxPhoneBookTest.hpp"
#include <CallMonitor/src/FritzBoxPhoneBook.hpp>
#include <QTest>

using namespace std;

static FritzBoxPhoneBookTest fritzBoxPhoneBookTest;

void FritzBoxPhoneBookTest::testRead() {
	// To prevent real names ending up on GitHub.com, this XML file has been edited (with Eclipse)
	// and the real names have been replaced with dummy names:

	QFile file( ":/FRITZ.Box_Telefonbuch.xml" );
	QVERIFY( file.open( QFile::ReadOnly | QFile::Text ));

	FritzBoxPhoneBook book;
	int count = book.read( &file );
	QCOMPARE( count, 6 );

	QCOMPARE( book.value( QStringLiteral( "1111111111" )), QStringLiteral( "FirstPerson" ));
	QCOMPARE( book.value( QStringLiteral( "2222222222" )), QStringLiteral( "SecondPerson" ));
	QCOMPARE( book.value( QStringLiteral( "3333333333" )), QStringLiteral( "ThirdPerson" ));
}
