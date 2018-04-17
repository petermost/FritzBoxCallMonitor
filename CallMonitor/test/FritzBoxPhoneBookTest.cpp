#include "FritzBoxPhoneBookTest.hpp"
#include <CallMonitor/src/FritzBoxPhoneBook.hpp>
#include <QTest>

using namespace std;

static FritzBoxPhoneBookTest fritzBoxPhoneBookTest;

void FritzBoxPhoneBookTest::testRead() {
	// To prevent real names ending up on GitHub.com, this XML file has been edited (with Eclipse)
	// and the real names have been replaced with dummy names:

	QString errorString;
	FritzBoxPhoneBook book;
	QVERIFY( book.read( ":/FRITZ.Box_Telefonbuch.xml", &errorString ));
	QCOMPARE( book.count(), 6 );

	QCOMPARE( book.findName( QStringLiteral( "1111111111" )), QStringLiteral( "FirstPerson" ));
	QCOMPARE( book.findName( QStringLiteral( "2222222222" )), QStringLiteral( "SecondPerson" ));
	QCOMPARE( book.findName( QStringLiteral( "3333333333" )), QStringLiteral( "ThirdPerson" ));
}

void FritzBoxPhoneBookTest::testFindNameOrDefault() {
	FritzBoxPhoneBook book;

	QString defaultName(QStringLiteral("defaultName"));
	QString nonExistingName(QStringLiteral("xxx"));
	QCOMPARE(book.findNameOrDefault(nonExistingName, defaultName), defaultName);
}
