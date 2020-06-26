#include "PhoneBook.hpp"
#include <QSqlError>
#include <QStandardPaths>

using namespace std;

const QString PhoneBook::CONNECTION_NAME(QStringLiteral("PhoneBook"));

QString PhoneBook::defaultFileName()
{
	return QStandardPaths::locate(QStandardPaths::AppDataLocation, CONNECTION_NAME);
}

PhoneBook::PhoneBook(QObject *parent)
	: QObject(parent)
{
	database_ = QSqlDatabase::addDatabase("QSQLITE", CONNECTION_NAME);
}

PhoneBook::~PhoneBook()
{
	QSqlDatabase::removeDatabase(CONNECTION_NAME);
}

bool PhoneBook::open(const QString &fileName)
{
	database_.setDatabaseName(fileName);
	if (!database_.open())
		throw database_.lastError();

	return true;
}


//QString PhoneBook::findNumber(const QString &name) const
//{
//	return entries_.value(name);
//}

//QString PhoneBook::findName(const QString &number) const
//{
//	auto nameIterator = entries_.find(number);
//	return (nameIterator != entries_.end()) ? *nameIterator : QString();
//}

//QString PhoneBook::findNameOrDefault(const QString &number, const QString &defaultName) const
//{
//	QString name = findName(number);
//	if (name.isEmpty())
//		name = defaultName;

//	return name;
//}

//int PhoneBook::count() const
//{
//	return entries_.count();
//}

//void PhoneBook::forEach(const function<void(const QString &, const QString &)> &consumer) const
//{
//	for (auto iterator = entries_.begin(); iterator != entries_.end(); ++iterator)
//		consumer(iterator.value(), iterator.key());
//}
