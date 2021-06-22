#pragma once

#include <QHash>
#include <QObject>
#include <QXmlStreamReader>
#include <functional>

class QIODevice;

class FritzBoxPhoneBook : public QObject {
	Q_OBJECT
	public:
		using Container = QHash<QString, QString>;

		FritzBoxPhoneBook(QObject *parent = nullptr) noexcept;

		/// Reads the phone book from the given file.
		bool read(const QString &fileName, QString *errorString);

		/// Reads the phone book from the given device.
		bool read(QIODevice *device, QString *errorString);

		/// Enter a number into the phone book.
		void enterName(const QString &name, const QString &number);

		/// Find the number for the name.
		QString findNumberForName(const QString &name) const;

		/// Find the name for the number.
		QString findNameForNumber(const QString &number) const;

		QString findNameForNumberOrDefault(const QString &number, const QString &defaultName) const;

		/// Returns the number of entries.
		int count() const;

		void forEach(const std::function<void(const QString &name, const QString &number)> &) const;

	private:
		Container entries_;
};
