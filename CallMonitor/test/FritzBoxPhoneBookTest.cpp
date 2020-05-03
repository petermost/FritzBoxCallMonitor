#include "FritzBoxPhoneBookTest.hpp"
#include <CallMonitor/src/FritzBoxPhoneBook.hpp>
#include <QTest>

using namespace std;

static FritzBoxPhoneBookTest fritzBoxPhoneBookTest;

void FritzBoxPhoneBookTest::testRead()
{
	// To prevent real names ending up on GitHub.com, this XML file has been edited (with Visual
	// Studio Code) and the real names have been replaced with dummy names:

	QString errorString;
	FritzBoxPhoneBook book;
	QVERIFY(book.read(":/FRITZ.Box_Telefonbuch.xml", &errorString));
	QCOMPARE(book.count(), 6);

	// One person can have multiple phone numbers:

	QCOMPARE(book.findName("1111111111"), "FirstPerson");
	QCOMPARE(book.findName("2222222222"), "FirstPerson");
	QCOMPARE(book.findName("3333333333"), "FirstPerson");

	QCOMPARE(book.findName("5555555555"), "SecondPerson");

	QCOMPARE(book.findName("6666666666"), "ThirdPerson");
}

void FritzBoxPhoneBookTest::testFindNameOrDefault()
{
	FritzBoxPhoneBook book;

	QString defaultName(QStringLiteral("defaultName"));
	QString nonExistingName(QStringLiteral("xxx"));
	QCOMPARE(book.findNameOrDefault(nonExistingName, defaultName), defaultName);
}
