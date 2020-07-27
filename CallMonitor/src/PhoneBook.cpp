#include "PhoneBook.hpp"
#include <QSqlError>
#include <QStandardPaths>
#include <pera_software/aidkit/qt/core/Strings.hpp>

using namespace std;
using namespace pera_software::aidkit::qt;

const QString PhoneBook::CONNECTION_NAME(QStringLiteral("PhoneBook"));

QString PhoneBook::defaultFileName()
{
	return QStandardPaths::locate(QStandardPaths::AppDataLocation, CONNECTION_NAME);
}

PhoneBook::PhoneBook(QObject *parent)
	: QObject(parent)
{
	database_ = QSqlDatabase::addDatabase("QSQLITE"_qs, CONNECTION_NAME);
}

PhoneBook::~PhoneBook()
{
	Q_ASSERT(!database_.isOpen());

	// TODO: database_ needs to be out of scope i.e. the destructor was called, otherwise we get
	// the runtime-warning:
	// "QSqlDatabasePrivate::removeDatabase: connection 'PhoneBook' is still in use, all queries will cease to work."

	QSqlDatabase::removeDatabase(CONNECTION_NAME);
}

bool PhoneBook::open(const QString &fileName)
{
	database_.setDatabaseName(fileName);
	if (!database_.open())
		throw database_.lastError();

	return true;
}

bool PhoneBook::close()
{
	database_.close();
	if (database_.isOpen())
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
