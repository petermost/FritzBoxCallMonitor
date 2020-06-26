#include <gtest/gtest.h>
#include <CallMonitor/src/PhoneBook.hpp>

using namespace std;

TEST(PhoneBookTest, testOpen)
{
	PhoneBook phoneBook;

	phoneBook.open(PhoneBook::defaultFileName());
}
