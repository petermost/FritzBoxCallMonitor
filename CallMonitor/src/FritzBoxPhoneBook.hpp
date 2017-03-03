#pragma once

#include <QObject>
#include <QHash>
#include <QXmlStreamReader>

class QIODevice;

class XmlException {
	public:
		XmlException( QXmlStreamReader::Error error, const QString &text )
			: error_( error ), text_( text ) {
		}

		QXmlStreamReader::Error error() const noexcept {
			return error_;
		}

		QString text() const {
			return text_;
		}

	private:
		QXmlStreamReader::Error error_;
		QString text_;
};

class FritzBoxPhoneBook : public QObject, public QHash< QString, QString >  {
	Q_OBJECT
	public:
		FritzBoxPhoneBook( QObject *parent = nullptr ) noexcept;

		int read( const QString &fileName );

		/// reads the phone book from the given device.
		/// Returns the number of entries read.
		int read( QIODevice *device );

	signals:
};
