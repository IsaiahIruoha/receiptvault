#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QString>
#include <QList>
#include <QPair>

class DatabaseManager
{
public:
    // singleton instance accessor
    static DatabaseManager& instance();

    //opens the database and creates tables if needed
    bool openDatabase(const QString &databasePath);

    // getter for the QSqlDatabase instance
    QSqlDatabase& getDatabase();

    // user related methods
    bool createUser(const QString &username, const QString &hashedPassword, const QString &salt);
    bool verifyUser(const QString &username, const QString &hashedPassword);
    bool getUserCredentials(const QString &username, QString &hashedPassword, QString &salt);

    // category related methods
    bool addCategory(const QString &categoryName);

    // expense related methods
    bool addExpense(int userId, int categoryId, const QString &store, const QString &items, const QString &date, double amount, const QString &description);
    QList<QPair<QString, double>> getCategoryExpenses(int userId);

private:
    // private constructor for Singleton
    DatabaseManager();

    // private destructor
    ~DatabaseManager();

    // disallow copy and assignment
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    // creates necessary tables
    bool createTables();

    QSqlDatabase db;
};

#endif
