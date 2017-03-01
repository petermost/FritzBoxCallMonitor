#include "FritzBoxPhoneBook.hpp"
#include <QIODevice>
#include <QXmlStreamReader>

using XmlToken = QXmlStreamReader::TokenType;

//==================================================================================================

FritzBoxPhoneBook::FritzBoxPhoneBook( QObject *parent ) noexcept
	: QObject( parent ) {
}

int FritzBoxPhoneBook::read( QIODevice *device ) {
	Q_ASSERT( device->isOpen() );

	clear();

	XmlToken token;
	QXmlStreamReader reader( device );
	QString name, number;

	while (( token = reader.readNext()) != XmlToken::Invalid ) {
		switch ( token ) {
			case XmlToken::StartElement: {
				auto elementName = reader.name();
				if ( elementName == "realName" ) {
					name = reader.readElementText();
				}
				if ( elementName == "number" ) {
					number = reader.readElementText();
					insert( number, name );
					name.clear();
					number.clear();
				}
				break;
			}
			default:
				break;
		}
	}
	if ( reader.atEnd() && reader.hasError() && reader.error() != QXmlStreamReader::Error::PrematureEndOfDocumentError ) {
		throw XmlException( reader.error(), reader.errorString() );
	}
	return count();
}

//==================================================================================================

