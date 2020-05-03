#include "FritzBoxPhoneBook.hpp"
#include <QFile>
#include <QIODevice>
#include <QXmlStreamReader>

using namespace std;

using XmlToken = QXmlStreamReader::TokenType;
//==================================================================================================

FritzBoxPhoneBook::FritzBoxPhoneBook(QObject *parent) noexcept
	: QObject(parent)
{
}

//==================================================================================================

bool FritzBoxPhoneBook::read(const QString &fileName, QString *errorString)
{
	QFile file(fileName);
	if (file.open(QFile::ReadOnly | QFile::Text))
		return read(&file, errorString);
	else {
		*errorString = file.errorString();
		return false;
	}
}

//==================================================================================================

bool FritzBoxPhoneBook::read(QIODevice *device, QString *errorString)
{
	Q_ASSERT(device->isOpen());

	entries_.clear();

	XmlToken token;
	QXmlStreamReader reader(device);
	QString name, number;

	while ((token = reader.readNext()) != XmlToken::Invalid) {
		switch (token) {
			case XmlToken::StartElement: {
				auto elementName = reader.name();
				if (elementName == "realName") {
					name = reader.readElementText();
				}
				if (elementName == "number") {
					number = reader.readElementText();
					entries_.insert(number, name);
					name.clear();
					number.clear();
				}
				break;
			}
			default:
				break;
		}
	}
	if (reader.atEnd() && reader.hasError() && reader.error() != QXmlStreamReader::Error::PrematureEndOfDocumentError) {
		*errorString = reader.errorString();
		return false;
	}
	return true;
}

//==================================================================================================

QString FritzBoxPhoneBook::findNumber(const QString &name) const
{
	return entries_.value(name);
}

//==================================================================================================

QString FritzBoxPhoneBook::findName(const QString &number) const
{
	auto nameIterator = entries_.find(number);
	return (nameIterator != entries_.end()) ? *nameIterator : QString();
}

//==================================================================================================

QString FritzBoxPhoneBook::findNameOrDefault(const QString &number, const QString &defaultName) const
{
	QString name = findName(number);
	if (name.isEmpty())
		name = defaultName;

	return name;
}

//==================================================================================================

int FritzBoxPhoneBook::count() const
{
	return entries_.count();
}

void FritzBoxPhoneBook::forEach(const function<void(const QString &, const QString &)> &consumer) const
{
	for (auto iterator = entries_.begin(); iterator != entries_.end(); ++iterator)
		consumer(iterator.value(), iterator.key());
}
