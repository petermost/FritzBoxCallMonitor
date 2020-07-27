#pragma once

#include <QSqlDatabase>
#include <QObject>
#include <functional>

class PhoneBook : public QObject {
	Q_OBJECT
	public:
		static QString defaultFileName();

		PhoneBook(QObject *parent = nullptr);
		~PhoneBook() override;

		bool open(const QString &fileName);
		bool close();

		/// Find the number for the name.
		QString findNumber(const QString &name) const;

		/// Find the name for the number.
		QString findName(const QString &number) const;

		QString findNameOrDefault(const QString &number, const QString &defaultName) const;

		/// Returns the number of entries.
		int count() const;

		void forEach(const std::function<void(const QString &name, const QString &number)> &) const;

	private:
		static const QString CONNECTION_NAME;

		QSqlDatabase database_;
};
