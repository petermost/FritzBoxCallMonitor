#include <gtest/gtest.h>
#include <CallMonitor/src/FritzBoxPhoneBook.hpp>
#include <pera_software/aidkit/qt/core/Strings.hpp>
#include <pera_software/aidkit/qt/core/StringPrinter.hpp>

using namespace std;
using namespace pera_software::aidkit::qt;

TEST(FritzBoxPhoneBookTest, testRead)
{
	// To prevent real names ending up on GitHub.com, this XML file has been edited (with Visual
	// Studio Code) and the real names have been replaced with dummy names:

	QString errorString;
	FritzBoxPhoneBook book;
	ASSERT_TRUE(book.read(":/FRITZ.Box_Telefonbuch.xml"_qs, &errorString));
	ASSERT_EQ(book.count(), 6);

	// One person can have multiple phone numbers:

	ASSERT_EQ(book.findNameForNumber("1111111111"_qs), "FirstPerson"_qs);
	ASSERT_EQ(book.findNameForNumber("2222222222"_qs), "FirstPerson"_qs);
	ASSERT_EQ(book.findNameForNumber("3333333333"_qs), "FirstPerson"_qs);

	ASSERT_EQ(book.findNameForNumber("5555555555"_qs), "SecondPerson"_qs);

	ASSERT_EQ(book.findNameForNumber("6666666666"_qs), "ThirdPerson"_qs);
}

TEST(FritzBoxPhoneBookTest, testFindSimilarNumber)
{
	FritzBoxPhoneBook book;
	book.enterName("Name"_qs, "+49 175 7814852"_qs);
	auto name = book.findNameForNumber("01755462157"_qs);
	ASSERT_EQ(name, "Name"_qs);
}

TEST(FritzBoxPhoneBookTest, testFindNameOrDefault)
{
	FritzBoxPhoneBook book;

	QString defaultName(QStringLiteral("defaultName"));
	QString nonExistingName(QStringLiteral("xxx"));
	ASSERT_EQ(book.findNameForNumberOrDefault(nonExistingName, defaultName), defaultName);
}
