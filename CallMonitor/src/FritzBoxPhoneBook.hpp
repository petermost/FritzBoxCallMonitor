#pragma once

#include <QObject>
#include <QHash>
#include <QXmlStreamReader>

class QIODevice;

class FritzBoxPhoneBook : public QObject, public QHash< QString, QString >  {
	Q_OBJECT
	public:
		FritzBoxPhoneBook( QObject *parent = nullptr ) noexcept;

		/// Reads the phone book from the given file.
		bool read( const QString &fileName , QString *errorString );

		/// Reads the phone book from the given device.
		bool read( QIODevice *device , QString *errorString );

	signals:
};
